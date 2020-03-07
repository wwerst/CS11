#include "mbrot.h"

/* Compute the mandelbrot iteration
 * and return a shared pointer to
 * MandelbrotPointInfo.
 */
SP_MandelbrotPointInfo compute_mandelbrot(d_complex c, int max_iters,
                                       bool collect_points) {
	auto mbp_info = std::make_shared<MandelbrotPointInfo>();
	mbp_info->max_iters = max_iters;
	size_t i = 0;
	d_complex z {0, 0};
	mbp_info->initial_point = z;
	for (i = 0; i < max_iters && std::norm(z) < 4; i++) {
		z = z*z + c;
		if (collect_points) {
			mbp_info->points_in_path.push_back(z);
		}
	}
	mbp_info->num_iters = i;
	mbp_info->escaped = (std::norm(z) > 4);
	return mbp_info;
}
