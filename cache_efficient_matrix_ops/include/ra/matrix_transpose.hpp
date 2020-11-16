#pragma once

#include <algorithm>
#include <utility>

namespace ra::cache {
namespace {
template <class T, class F>
void compute_in_place(F transpose_op, std::size_t m, std::size_t n, T *b) {
 	// Inefficient since we are initializing n * m class type T objects
	std::unique_ptr<T> c(new T[m * n]);
  transpose_op(c.get());
	// Results are copied element wise instead of swapping pointers
	// since b may be an array on the stack of the caller.
	// Swapping b, c and subsequently deleting c would result in calling
	// delete on a stack variable, which results in UB
	// Bad cache efficiency due to large stride retrieval
	for (std::size_t i = 0; i < m; ++i) {
		for (std::size_t j = 0; j < n; ++j) {
			// Perhaps this is horribly inefficient (call to get())
			b[i * n + j] = c.get()[i * n + j];
		}
	}
  return;
}
template <class T>
void matrix_transpose_helper(const T *a, std::size_t m_orig, std::size_t n_orig,
                             std::size_t m, std::size_t n, T *b) {
  if (a == b) {
    compute_in_place(
        [a, m, n](T *c) { matrix_transpose_helper(a, m, n, m, n, c); }, m, n,
        b);
    return;
  }

  if (m * n <= 64) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        b[j * m_orig + i] = a[i * n_orig + j];
      }
    }
    return;
  }

  if (m >= n) {
    std::size_t m_half = m / 2;
    matrix_transpose_helper(a, m_orig, n_orig, m_half, n, b);
    matrix_transpose_helper(a + m_half * n_orig, m_orig, n_orig, m - m_half, n,
                            b + m_half);
  } else {
    std::size_t n_half = n / 2;
    matrix_transpose_helper(a, m_orig, n_orig, m, n_half, b);
    matrix_transpose_helper(a + n_half, m_orig, n_orig, m, n - n_half,
                            b + m_orig * n_half);
  }
}
} // namespace

template <class T>
void matrix_transpose(const T *a, std::size_t m, std::size_t n, T *b) {
  // Need to propagate dimensions of outermost matrix
  matrix_transpose_helper(a, m, n, m, n, b);
};

template <class T>
void naive_matrix_transpose(const T *a, std::size_t m, std::size_t n, T *b) {
  if (a == b) {
    compute_in_place([a, m, n](T *c) { naive_matrix_transpose(a, m, n, c); }, m,
                     n, b);
    return;
  }
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      // Throws if copy assignment of T throws
      // Nothing to do here if it does, propagate exception
      b[j * m + i] = a[i * n + j];
    }
  }
};
} // namespace ra::cache
