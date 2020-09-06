#include "cexpr_basic_string.hpp"

#include <cstddef>

namespace ra::fractal {
using ra::cexpr::cexpr_string;
using ra::cexpr::to_string;

// Using the below complex class yields surprisingly slow computation
// of the set. I suspect this is because the constexpr code is interpreted, 
// i.e. no function call inlined, etc.
//
/*
class Complex {
public:
  constexpr Complex(double r, double i) : re(r), im(i){};
  constexpr Complex() : re(0), im(0){};
  constexpr Complex operator*(const Complex &z) const {
    return Complex(re * z.re - im * z.im, re * z.im + im * z.re);
  }
  constexpr Complex operator+(const Complex &z) const {
    return Complex(re + z.re, im + z.im);
  }
  constexpr double modulus() const { return std::sqrt(re * re + im * im); }

private:
  double re;
  double im;
};
*/

constexpr bool in_set(double a0, double a1, double b0, double b1, std::size_t W,
                      std::size_t H, std::size_t k, std::size_t l) {
  // Sample region around Mandelbrot set to obtain first element c in sequence
  double c_re = a0 + k * (b0 - a0) / (W - 1);
  double c_im = a1 + (H - 1 - l) * (b1 - a1) / (H - 1);
  double prev_re = c_re;
  double prev_im = c_im;
  

  // For simplicity, assume sequence diverges for mod(z) > 2.0
  // and converges if it hasn't diverged by the 16th iteration
  for (int i = 1; i < 16; ++i) {
    if (prev_re * prev_re + prev_im * prev_im > 4.0) {
      return false;
    }
    double next_re = prev_re * prev_re - prev_im * prev_im + c_re;
    double next_im = 2 * prev_re * prev_im + c_im; 
    prev_re = next_re;
    prev_im = next_im;
  }
  return true;
};

template <std::size_t W, std::size_t H>
constexpr cexpr_string<W * H + 2 * H> compute_mandelbrot_set() {
  // Convert W, H into cstrings
  char width_s[256] = {};
  char height_s[256] = {};
  char *end = width_s;
  to_string(W, width_s, 256, &end);
  to_string(H, height_s, 256, &end);

  // Initialization of mbset string
  auto mbset = cexpr_string<W * H + 2 * H>("P1 ");
  mbset.append(width_s);
  mbset.append(" ");
  mbset.append(height_s);
  mbset.append("\n");

  char buffer[2] = {0};
  for (std::size_t l = 0; l < H; ++l) {
    for (std::size_t k = 0; k < H; ++k) {
      bool member = in_set(-1.6, -1.1, 0.6, 1.1, W, H, k, l);
      to_string(member, buffer, 2, &end);
      mbset.append(buffer);
    }
    mbset.append("\n");
  }

  return mbset;
};

template <std::size_t W, std::size_t H>
constexpr auto mandelbrot = compute_mandelbrot_set<W, H>();
} // namespace ra::fractal
