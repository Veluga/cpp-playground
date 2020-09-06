#include "ra/mandelbrot.hpp"
#include <iostream>

int main() {
  constexpr auto s = ra::fractal::mandelbrot<255, 255>;

  std::cout << s.begin() << '\n';
}
