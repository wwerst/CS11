#include "bbrot.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv) {
	if (argc != 4) {
		std::cerr << "Incorrect number of arguments, expected ./bbrot image_size samples iter_count\n";
		exit(1);
	}
	int image_size = atoi(argv[1]);
	int num_points = atoi(argv[2]);
	int max_iter_count = atoi(argv[3]);
	Image image { image_size, image_size };
	std::default_random_engine rand_engine {};
	std::uniform_real_distribution<> real_d(-2, 1);
	std::uniform_real_distribution<> imag_d(-1.5, 1.5);
	for (size_t i = 0; i < num_points; i++) {
		d_complex c {real_d(rand_engine), imag_d(rand_engine)};
		auto mbp_info = compute_mandelbrot(c, max_iter_count, true);
		if (mbp_info.escaped) {
			update_image(image, mbp_info);
		}
	}
	output_image_to_pgm(image, std::cout);
}

double normalize(double min, double max, double value) {
	assert(min < max);
	return (value - min) / (max - min);
}

void update_image(Image &image, const MandelbrotPointInfo &info) {
	for (d_complex p : info.points_in_path) {
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
