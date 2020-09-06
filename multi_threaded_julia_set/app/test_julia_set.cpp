#include "ra/julia_set.hpp"

#include <iostream>

using namespace ra::fractal;

int main() {
	std::complex<double> c(-0.420, 0.69);
	std::complex<double> bottom_left(-1.25, -1.25);
	std::complex<double> top_right(1.25, 1.25);
	int max_iters = 255;
	int num_threads = 8;
	std::size_t width = 2048;
	std::size_t height = 2048;
	boost::multi_array<int, 2> a(boost::extents[height][width]);
  compute_julia_set<double>(bottom_left, top_right, c, max_iters, a, num_threads);

	std::cout << "P2 " << width << " " << height << " " << max_iters << "\n";
	for (int i = height - 1; i >= 0; --i) {
		for (int j = 0; j < width; ++j) {
			std::cout << a[i][j] << " ";
		}
		std::cout << "\n";
	}
	return 0;
}

