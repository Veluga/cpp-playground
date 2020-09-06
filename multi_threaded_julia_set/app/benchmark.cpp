#include <benchmark/benchmark.h>

#include "ra/julia_set.hpp"

using namespace ra::fractal;

static void BM_compute_julia(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
		std::complex<double> c(0.37, -0.16);
		std::complex<double> bottom_left(-1.25, -1.25);
		std::complex<double> top_right(1.25, 1.25);
		int max_iters = 255;
		int num_threads = state.range(1);
		std::size_t width = state.range(0);
		std::size_t height = state.range(0);
		boost::multi_array<int, 2> a(boost::extents[height][width]);
    state.ResumeTiming();
  	compute_julia_set<double>(bottom_left, top_right, c, max_iters, a, num_threads);
  }
}

BENCHMARK(BM_compute_julia)->Ranges({{512, 4096}, {1, 8}});;

BENCHMARK_MAIN();
