#define CATCH_CONFIG_MAIN

#include <ra/fft.hpp>

#include <catch2/catch.hpp>
#include <complex>
#include <random>

using namespace ra::cache;

template <class T> 
void check_vector_equal(T *a, T *b, std::size_t n) {
	for (std::size_t i = 0; i < n; ++i) {
		REQUIRE(a[i].real() == Approx(b[i].real()).margin(1e-12));
		REQUIRE(a[i].imag() == Approx(b[i].imag()).margin(1e-12));
	}
};

template <class T>
T* generate_random_vector(std::size_t n) {
  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_real_distribution<> dis(-1.0, 1.0);

	T* v = new T[n];
	for (int i = 0; i < n; ++i) {
		v[i] = T(dis(rng), dis(rng));
	}
	return v;
};

template <class T>
T* copy_vector(const T* v, std::size_t n) {
	T* copy = new T[n];
	for (int i = 0; i < n; ++i) {
		copy[i] = v[i];
	}
	return copy;
}

TEST_CASE("Naive FFT.") {
  SECTION("Two elements.") {
		auto x = generate_random_vector<std::complex<double>>(2);
		std::complex<double> expected[2] = {
			x[0] + x[1],
			x[0] - x[1]
		};
		
		naive_fft<std::complex<double>>(x, 2);
		check_vector_equal(x, expected, 2);
  }

  SECTION("Four elements.") {
		auto x = generate_random_vector<std::complex<double>>(4);
		std::complex<double> expected[4] = {
			x[0] + x[1] + x[2] + x[3],
			x[0] - x[2] + std::complex<double>(0, 1) * (-x[1] + x[3]),
			x[0] - x[1] + x[2] - x[3],
			x[0] - x[2] + std::complex<double>(0, 1) * (x[1] - x[3])
		};

		naive_fft<std::complex<double>>(x, 4);
		check_vector_equal(x, expected, 4);
  }
}

TEST_CASE("Cache oblivious FFT.") {
  SECTION("Two elements.") {
		auto x = generate_random_vector<std::complex<double>>(2);
		auto expected = copy_vector(x, 2);
		
		forward_fft<std::complex<double>>(x, 2);
		naive_fft<std::complex<double>>(expected, 2);
		check_vector_equal(x, expected, 2);
  }
	
  SECTION("Four elements.") {
		auto x = generate_random_vector<std::complex<double>>(4);
		auto expected = copy_vector(x, 4);
		
		forward_fft<std::complex<double>>(x, 4);
		naive_fft<std::complex<double>>(expected, 4);
		check_vector_equal(x, expected, 4);
  }

  SECTION("Eight elements.") {
		auto x = generate_random_vector<std::complex<double>>(8);
		auto expected = copy_vector(x, 8);
		
		forward_fft<std::complex<double>>(x, 8);
		naive_fft<std::complex<double>>(expected, 8);
		check_vector_equal(x, expected, 8);
  }

  SECTION("128 elements.") {
		auto x = generate_random_vector<std::complex<double>>(128);
		auto expected = copy_vector(x, 128);
		
		forward_fft<std::complex<double>>(x, 128);
		naive_fft<std::complex<double>>(expected, 128);
		check_vector_equal(x, expected, 128);
  }

  SECTION("4096 elements.") {
		auto x = generate_random_vector<std::complex<double>>(4096);
		auto expected = copy_vector(x, 4096);
		
		forward_fft<std::complex<double>>(x, 4096);
		naive_fft<std::complex<double>>(expected, 4096);
		check_vector_equal(x, expected, 4096);
  }
}
