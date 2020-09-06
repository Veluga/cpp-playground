#include <algorithm>
#include <random>
#include <memory>

namespace ra::cache {

template <class T>
std::unique_ptr<T> random_matrix(std::size_t m, std::size_t n) {
  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_real_distribution<> dis(-1.0, 1.0);

  std::unique_ptr<T> mat(new T[m * n]);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      mat.get()[i * n + j] = T(dis(rng));
    }
  }
  return mat;
}

template <class T>
std::unique_ptr<T> copy_matrix(T *a, std::size_t m, std::size_t n) {
  std::unique_ptr<T> b(new T[m * n]);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      b.get()[i * n + j] = a[i * n + j];
    }
  }
  return b;
}

template <class T>
void matrix_multiply_helper(const T *a, const T *b, std::size_t m_orig,
                            std::size_t n_orig, std::size_t p_orig,
                            std::size_t m, std::size_t n, std::size_t p, T *c) {
  if (m * n * p <= T(64)) {
    // Base case
    for (int i = 0; i < m; ++i) {
      for (int k = 0; k < p; ++k) {
        T sum(0);
        for (int j = 0; j < n; ++j) {
          sum += a[i * n_orig + j] * b[j * p_orig + k];
        }
        c[i * p_orig + k] += sum;
      }
    }
		return;
  }

  if (m == std::max({m, n, p})) {
    // Halve m
    std::size_t m_half = m / 2;
    matrix_multiply_helper(a, b, m_orig, n_orig, p_orig, m_half, n, p, c);
    matrix_multiply_helper(a + m_half * n_orig, b, m_orig, n_orig, p_orig,
                           m - m_half, n, p, c + m_half * p_orig);
  } else if (n == std::max({m, n, p})) {
    // Halve n
    std::size_t n_half = n / 2;
    matrix_multiply_helper(a, b, m_orig, n_orig, p_orig, m, n_half, p, c);
    matrix_multiply_helper(a + n_half, b + n_half * p_orig, m_orig, n_orig,
                           p_orig, m, n - n_half, p, c);
  } else {
    // Halve p
    std::size_t p_half = p / 2;
    matrix_multiply_helper(a, b, m_orig, n_orig, p_orig, m, n, p_half, c);
    matrix_multiply_helper(a, b + p_half, m_orig, n_orig, p_orig, m, n,
                           p - p_half, c + p_half);
  }
}

template <class T>
void matrix_multiply(const T *a, const T *b, std::size_t m, std::size_t n,
                     std::size_t p, T *c) {
  matrix_multiply_helper(a, b, m, n, p, m, n, p, c);
}

template <class T>
void naive_matrix_multiply(const T *a, const T *b, std::size_t m, std::size_t n,
                           std::size_t p, T *c) {
  for (int i = 0; i < m; ++i) {
    for (int k = 0; k < p; ++k) {
      T sum(0);
      for (int j = 0; j < n; ++j) {
        sum += a[i * n + j] * b[j * p + k];
      }
      c[i * p + k] = sum;
    }
  }
}
} // namespace ra::cache
