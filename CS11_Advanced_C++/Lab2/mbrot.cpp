#include "mbrot.h"

MandelbrotPointInfo compute_mandelbrot(d_complex c, int max_iters,
                                       bool collect_points) {
	// Iterate the Mandelbrot recursive equation Zn_1 = Zn^2 + c.
	// Iterates for max_iters and returns MandelbrotPointInfo
	// describing results of iteration. If collect_points is true,
	// the found points are returned as well.
	auto mbp_info = MandelbrotPointInfo {};
	mbp_info.max_iters = max_iters;
	size_t i = 0;
	d_complex z {0, 0};
	mbp_info.initial_point = z;
	for (i = 0; i < max_iters && std::norm(z) < 4; i++) {
		z = z*z + c;
		if (collect_points) {
			mbp_info.points_in_path.push_back(z);
		}
	}
	mbp_info.num_iters = i;
	mbp_info.escaped = (std::norm(z) > 4);
	return mbp_info;
}
