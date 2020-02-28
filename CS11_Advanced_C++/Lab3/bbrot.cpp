#include "bbrot.h"
#include <algorithm>
#include <cassert>
#include <thread>
#include <vector>

void generate_bbot_trajectories(int num_points, int max_iters,
								std::shared_ptr<ConcurrentBoundedQueue> queue) {
	std::default_random_engine rand_engine {};
	std::uniform_real_distribution<> real_d(-2, 1);
	std::uniform_real_distribution<> imag_d(-1.5, 1.5);
	int generated_trajs = 0;
	while (generated_trajs < num_points) {
		d_complex c {real_d(rand_engine), imag_d(rand_engine)};
		auto mbp_info = compute_mandelbrot(c, max_iters, true);
		if (mbp_info->escaped) {
			++generated_trajs;
			queue->put(mbp_info);
		}
	}
	queue->put(nullptr);
}

int main(int argc, char **argv) {
	if (argc != 5) {
		std::cerr << "Incorrect number of arguments, expected ./bbrot image_size samples iter_count num_threads\n";
		exit(1);
	}
	int image_size = atoi(argv[1]);
	int num_points = atoi(argv[2]);
	int max_iters = atoi(argv[3]);
	int num_threads = atoi(argv[4]);
	auto queue = std::make_shared<ConcurrentBoundedQueue>(1000);
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
	for (std::thread& t : threads) {
		t.join();
	}
	output_image_to_pgm(image, std::cout);
}


double normalize(double min, double max, double value) {
	assert(min < max);
	return (value - min) / (max - min);
}

void update_image(Image &image, const SP_MandelbrotPointInfo info) {
	for (d_complex p : info->points_in_path) {
		double norm_real = normalize(-2, 1, p.real());
		double norm_imag = normalize(-1.5, 1.5, p.imag());
		if (norm_real < 0 || norm_real > 1 || norm_imag < 0 || norm_imag > 1) {
			continue;
		} else {
			size_t x = norm_real * (image.getWidth()-1);
			size_t y = norm_imag * (image.getHeight()-1);
			image.incValue(x, y);
		}
	}

}

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
