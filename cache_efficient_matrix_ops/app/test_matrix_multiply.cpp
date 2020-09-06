#define CATCH_CONFIG_MAIN

#include <ra/matrix_multiply.hpp>

#include <catch2/catch.hpp>
#include <memory>

using namespace ra::cache;

template <class T> void check_matrix_equal(T *a, T *b, std::size_t m, std::size_t n) {
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
			REQUIRE(a[i * n + j] == Approx(b[i * n + j]));
    }
  }
};

TEST_CASE("Matrix multiply.") {
  SECTION("Naive.") {
    double a[] = {1, 2, 3, 4};
    double b[] = {5, 6, 7, 8, 9, 10};
    double c[6] = {0};
    naive_matrix_multiply(a, b, 2, 2, 3, c);
    double res[] = {21, 24, 27, 47, 54, 61};
    check_matrix_equal(c, res, 2, 3);
  }

  SECTION("Cache efficient.") {
    std::unique_ptr<double> a = random_matrix<double>(100, 100);
    std::unique_ptr<double> b = random_matrix<double>(100, 200);
    std::unique_ptr<double> c(new double[100 * 200]());

    std::unique_ptr<double> d = copy_matrix<double>(a.get(), 100, 100);
    std::unique_ptr<double> e = copy_matrix<double>(b.get(), 100, 200);
    std::unique_ptr<double> f = copy_matrix<double>(c.get(), 100, 200);

		matrix_multiply(a.get(), b.get(), 100, 100, 200, c.get());
    naive_matrix_multiply(d.get(), e.get(), 100, 100, 200, f.get());

    check_matrix_equal(c.get(), f.get(), 100, 200);
  }
}
