#include "bbrot.h"
#include <algorithm>
#include <cassert>
#include <getopt.h>
#include <thread>
#include <vector>

/* Generate trajectories and add them to a thread-safe queue.
 * This is the target of worker threads for multi-threaded
 * rendering.
 */
void generate_bbot_trajectories(int num_points, int max_iters,
                                std::shared_ptr<ConcurrentBoundedQueue<SP_MandelbrotPointInfo>> queue) {
    std::default_random_engine rand_engine {};
    std::uniform_real_distribution<> real_d(-2, 1);
    std::uniform_real_distribution<> imag_d(-1.5, 1.5);
    int generated_trajs = 0;
    while (generated_trajs < num_points) {
        d_complex c {real_d(rand_engine), imag_d(rand_engine)};
        auto mbp_info = compute_mandelbrot(c, max_iters, false);
        if (!mbp_info->escaped) {
            ++generated_trajs;
            queue->put(mbp_info);
        }
    }
    queue->put(nullptr);
}

/* Print program usage message, and exit(1) from program.
 */
void print_usage_exit(char* prog_name) {
    std::cerr << "usage : " << prog_name << " [-s|--size <image_size>] [-p|--points <num_points>]\n"
"        [-i|--iters <max_iters>] [-t|--threads <num_threads>]\n"
"        Generate Buddhabrot renderings in pgm grayscale images. Image data is output to stdout.\n"
"\n"
"        -s|--size      Image size in pixels. Default: 800:\n"
"        -p|--points    Total number of random starting points generated. Default: 1,000,000\n"
"        -i|--iters     Maximum iteration limit before a point is declared to be in the set. Default: 1,000\n"
"        -t|--threads   Number of producer threads to run. Defaults to number of available logical cores.\n";
    exit(1);
}

/* Parse string to passed reference integer
 * Return true if successful, else false
 */
bool parse_str_to_int(char* str, int& target_int) {
    char* arg_parse_end;
    target_int = strtol(optarg, &arg_parse_end, /* base */ 10);
    return arg_parse_end == 0;
}

int main(int argc, char **argv) {
    // Parse arguments
    int image_size = 800;
    int num_points = 1000000;
    int max_iters = 1000;
    int num_threads = std::thread::hardware_concurrency();

    int opt_c = 0;
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"size", required_argument, 0, 's'},
            {"points", required_argument, 0, 'p'},
            {"iters", required_argument, 0, 'i'},
            {"threads", required_argument, 0, 't'}};
        opt_c = getopt_long(argc, argv, "s:p:i:t:", long_options, &option_index);
        if (opt_c == -1) {
            // Reached the end of option processing
            break;
        }
        switch (opt_c) {
            case 's':
                if (parse_str_to_int(optarg, image_size) || image_size <= 0) {
                    print_usage_exit(argv[0]);
                }
                break;
            case 'p':
                if (parse_str_to_int(optarg, num_points) || num_points <= 0) {
                    print_usage_exit(argv[0]);
                }
                break;
            case 'i':
                if (parse_str_to_int(optarg, max_iters) || max_iters <= 0) {
                    print_usage_exit(argv[0]);
                }
                break;
            case 't':
                if (parse_str_to_int(optarg, num_threads) || num_threads <= 0) {
                    print_usage_exit(argv[0]);
                }
                break;
            default:
                // Argument could not be processed
                print_usage_exit(argv[0]);
        }
    }
    // Print configuration of program
    std::cerr << "Running " << argv[0] << "with the following configuration:\n"
        "size: " << image_size << "\n"
        "points: " << num_points << "\n"
        "iters: " << max_iters << "\n"
        "producer threads: " << num_threads << "\n";

    // Initialize worker threads
    auto queue = std::make_shared<ConcurrentBoundedQueue<SP_MandelbrotPointInfo>>(1000);
    Image image { image_size, image_size };
    std::vector<std::thread> threads;
    int unallocated_points = num_points;
    for (size_t i = 0; i < num_threads; ++i) {
        int thread_num_points = std::min(1+(num_points / num_threads), unallocated_points);
        unallocated_points -= thread_num_points;
        threads.push_back(
            std::thread(generate_bbot_trajectories, thread_num_points, max_iters, queue));
    }
    assert(unallocated_points == 0);
    
    // Process points as they are generated by worker threads.
    int recv_points = 0;
    int terminated_threads = 0;
    while (terminated_threads < num_threads) {
        auto mbp_info = queue->get();
        if (mbp_info != nullptr) {
            ++recv_points;
            update_image(image, mbp_info);
        } else {
            ++terminated_threads;
        }
        if (recv_points % 1000000 == 0) {
            std::cerr << ".";
        }
    }
    std::cerr << "\n";
    assert(recv_points == num_points);
    // Join worker threads back.
    for (std::thread& t : threads) {
        t.join();
    }
    output_image_to_pgm(image, std::cout);
}

/* Linearly map (min, max) -> (0, 1)
 */
double normalize(double min, double max, double value) {
    assert(min < max);
    return (value - min) / (max - min);
}

/* Update image with new SP_MandelbrotPointInfo
 */
void update_image(Image &image, const SP_MandelbrotPointInfo info) {
    d_complex p = info->initial_point;
    // for (d_complex p : info->points_in_path) {
    double norm_real = normalize(-2, 1, p.real());
    double norm_imag = normalize(-1.5, 1.5, p.imag());
    if (norm_real < 0 || norm_real > 1 || norm_imag < 0 || norm_imag > 1) {
    } else {
        size_t x = norm_real * (image.getWidth()-1);
        size_t y = norm_imag * (image.getHeight()-1);
        image.incValue(x, y);
    }
    // }
}

/* Output image to output stream. This output stream should be
 * redirected to an image file.
 */
void output_image_to_pgm(const Image &image, std::ostream &os) {
    int max_image_val = 0;
    for (size_t y = 0; y < image.getHeight(); y++) {
        for (size_t x = 0; x < image.getWidth(); x++) {
            max_image_val = std::max(max_image_val, image.getValue(x, y));
        }
    }
    os << "P2 " << image.getWidth() << " ";
    os << image.getHeight() << " 255\n"; 
    for (size_t y = 0; y < image.getHeight(); y++) {
        for (size_t x = 0; x < image.getWidth(); x++) {
            os << (int)(255*normalize(0, max_image_val, image.getValue(x, y)));
            os << " ";
        }
        os << "\n";
    }
}
