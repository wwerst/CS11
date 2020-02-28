#include "cbqueue.h"
#include "image.h"
#include "mbrot.h"
#include <iostream>
#include <cstdlib>
#include <random>

void generate_bbot_trajectories(int num_points, int max_iters,
								std::shared_ptr<ConcurrentBoundedQueue> queue);
double normalize(double min, double max, double value);
void update_image(Image &image, const SP_MandelbrotPointInfo info);
void output_image_to_pgm(const Image &image, std::ostream &os);
