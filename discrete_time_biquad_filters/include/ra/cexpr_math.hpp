#include <boost/math/constants/constants.hpp>

#include <exception>
#include <limits>

namespace ra::cexpr_math {
template <class T, class Out = int> constexpr Out trunc(T x) {
  // This is very imprecise for large x but sufficient for the scope
  // of this assignment 
  return static_cast<Out>(x);
}

template <class T> constexpr T pi = boost::math::constants::pi<T>();

template <class T> constexpr T abs(T x) { return (x < 0 ? -x : x); };

template <class T> constexpr T sqr(T x) { return x * x; };

template <class T> constexpr T cube(T x) { return x * x * x; };

template <class T> constexpr T mod(T x, T y) {
  if (y == 0) {
    throw std::overflow_error("Modulo 0 is not defined");
  }
  int n = trunc<T>(x / y);
  return x - n * y;
};

template <class T> constexpr T sin(T x) {
  if (x < 0) {
    return -sin<T>(-x);
  }
  if (x <= 0.000001) {
    return x;
  }
  x = mod<T>(x, 2 * pi<T>);
  return 3 * sin<T>(x / 3) - 4 * cube<T>(sin<T>(x / 3));
};

template <class T> constexpr T cos(T x) { return sin<T>(x + pi<T> * 0.5); };

template <class T> constexpr T tan(T x) {
  T s = sin<T>(x);
  T c = cos<T>(x);
  if (c == 0) {
    throw std::overflow_error("tan(x) is undefined for cos(x) = 0");
  }
  return s / c;
};

template <class T> constexpr T sqrt(T x) {
  if (x <= 0) {
    return (x == 0 ? 0
                   : throw std::domain_error(
                         "Unable to compute square root of negative number."));
  }
  T x_prev = x;
  T x_n = x_prev - (x_prev * x_prev - x) / (2 * x_prev);
  while (abs<T>(x_n - x_prev) > std::numeric_limits<T>::epsilon()) {
    x_prev = x_n;
    x_n = x_prev - (x_prev * x_prev - x) / (2 * x_prev);
  }
  return x_n;
};
} // namespace ra::cexpr_math
