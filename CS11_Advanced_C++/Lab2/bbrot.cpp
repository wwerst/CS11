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
}

double normalize(double min, double max, double value) {
	return 0.0;
}

void update_image(Image &image, const MandelbrotPointInfo &info) {

}

void output_image_to_pgm(const Image &image, std::ostream &os) {

}
