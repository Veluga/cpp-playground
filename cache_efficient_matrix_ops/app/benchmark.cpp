#include <benchmark/benchmark.h>

#include "ra/matrix_transpose.hpp"
#include "ra/matrix_multiply.hpp"

using namespace ra::cache;

static void BM_naive_transpose(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    double *a = new double[1'000'000];
    state.ResumeTiming();
    naive_matrix_transpose(a, 1000, 1000, a);
    state.PauseTiming();
    delete [] a;
    state.ResumeTiming();
  }
}

static void BM_transpose(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    double *a = new double[1'000'000];
    state.ResumeTiming();
    naive_matrix_transpose(a, 1000, 1000, a);
    state.PauseTiming();
    delete [] a;
    state.ResumeTiming();
  }
}

static void BM_naive_multiply(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
		std::unique_ptr<double> a = random_matrix<double>(512, 512);
		std::unique_ptr<double> b = random_matrix<double>(512, 512);
		std::unique_ptr<double> c = random_matrix<double>(512, 512);
		state.ResumeTiming();
		naive_matrix_multiply(a.get(), b.get(), 512, 512, 512, c.get());
  }
}


static void BM_multiply(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
		std::unique_ptr<double> a = random_matrix<double>(512, 512);
		std::unique_ptr<double> b = random_matrix<double>(512, 512);
		std::unique_ptr<double> c = random_matrix<double>(512, 512);
		state.ResumeTiming();
		matrix_multiply(a.get(), b.get(), 512, 512, 512, c.get());
  }
}

BENCHMARK(BM_naive_transpose);
BENCHMARK(BM_transpose);
BENCHMARK(BM_naive_multiply);
BENCHMARK(BM_multiply);

BENCHMARK_MAIN();
