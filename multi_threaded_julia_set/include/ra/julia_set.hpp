#include "boost/multi_array.hpp"
#include "ra/thread_pool.hpp"

#include <complex>
#include <array>
#include <iostream>

namespace ra::fractal {
template <class Real>
std::complex<Real> initial_z(Real h_offset, Real w_offset, Real height, Real width,
                             const std::complex<Real> &bottom_left,
                             const std::complex<Real> &top_right) {
  Real real = bottom_left.real() + (w_offset / (width - 1)) *
                                       (top_right.real() - bottom_left.real());
  Real imag = bottom_left.imag() + (h_offset / (height - 1)) *
                                       (top_right.imag() - bottom_left.imag());
  return std::complex<Real>(real, imag);
};

template <class Real>
int gamma_m(const std::complex<Real> &z0, const std::complex<Real> &c, int m) {
  auto z = z0;
  for (int i = 0; i < m; ++i) {
    if (std::abs(z) > 2) {
      return i;
    }
    z = z * z + c;
  }
  return m;
};

template <class Real>
void compute_julia_set(const std::complex<Real> &bottom_left,
                       const std::complex<Real> &top_right,
                       const std::complex<Real> &c, int max_iters,
                       boost::multi_array<int, 2> &a, int num_threads) {
  using ra::concurrency::thread_pool;

  thread_pool tp(num_threads);
	int height = a.shape()[0];
	int width = a.shape()[1];
  for (int h_offset = 0; h_offset < height; ++h_offset) {
    tp.schedule([&, h_offset]() {
      for (int w_offset = 0; w_offset < width; ++w_offset) {
        std::complex<Real> z0 = initial_z<Real>(h_offset, w_offset, height, width,
                                          bottom_left, top_right);
        a[h_offset][w_offset] = gamma_m<Real>(z0, c, max_iters);
      }
    });
  }
	tp.shutdown();
};
} // namespace ra::fractal
