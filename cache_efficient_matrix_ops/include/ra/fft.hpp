#include <cstddef>
#include <complex>
#include <cmath>
#include <iostream>

#include "matrix_transpose.hpp"

namespace ra::cache {
template <class T>
constexpr T pi = 3.14159265358979323846;

template <class T>
T* dit_fft(T* x, std::size_t n, int stride = 1)
{
	if (n == 1) {
		T* res = new T[1];
		res[0] = x[0];
		return res;
	}
	T* lower = dit_fft(x, n/2, 2*stride);
	T* upper = dit_fft(x+stride, n/2, 2*stride);
	T* res = new T[n];

	for (int k = 0; k < n/2; ++k) {
		T twiddle_factor = std::polar<typename T::value_type>(1.0, -2 * pi<typename T::value_type> * k / n) * upper[k];
		res[k] = lower[k] + twiddle_factor;
		res[k + n/2] = lower[k] - twiddle_factor;
	}
	delete[] lower;
	delete[] upper;

	return res;
}

template <class T>
void naive_fft(T* x, std::size_t n)
{
	T* res = dit_fft(x, n);
	for (int i = 0; i < n; ++i) {
		x[i] = res[i];
	}
	delete[] res;
}

template <class T>
void forward_fft(T* x, std::size_t n)
{
	if (n <= 2) {
		return naive_fft<T>(x, n);
	}

	int n1 = std::pow(2, std::ceil(std::log2(n)/2));
	int n2 = std::pow(2, std::floor(std::log2(n)/2));

	matrix_transpose<T>(x, n1, n2, x);

	for (int i = 0; i < n2; ++i) {
		forward_fft<T>(x + n1 * i, n1);
	}

	T base_twiddle_factor = std::polar<typename T::value_type>(1.0, 2 * pi<typename T::value_type> / n);
	for (int i = 0; i < n2; ++i) {
		for (int j = 0; j < n1; ++j) {
			x[j + n1 * i] *= std::pow<typename T::value_type>(base_twiddle_factor, std::complex<typename T::value_type>(-i*j));
		}
	}

	matrix_transpose<T>(x, n2, n1, x);

	for (int i = 0; i < n1; ++i) {
		forward_fft<T>(x + n2 * i, n2);
	}

	matrix_transpose<T>(x, n1, n2, x);
}
}
