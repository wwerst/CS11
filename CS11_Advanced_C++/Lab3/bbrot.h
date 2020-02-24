#include "image.h"
#include "mbrot.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <random>

double normalize(double min, double max, double value);
void update_image(Image &image, const MandelbrotPointInfo &info);
void output_image_to_pgm(const Image &image, std::ostream &os);
