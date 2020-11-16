cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED)
find_package(benchmark REQUIRED)

project(matrix_cache LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_GCC "Use g++ or clang++." true)
if(USE_GCC)
  set(CMAKE_CXX_COMPILER "/usr/bin/g++")
else()
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

include(Sanitizers.cmake)

option(ENABLE_COVERAGE "Enable coverage" false)
option(ENABLE_DEBUG "Enable debug." false)

if(ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type." FORCE)
  include(CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()


add_executable(benchmark_test app/benchmark.cpp)
target_link_libraries(benchmark_test benchmark::benchmark)
target_include_directories(benchmark_test PUBLIC include)
set_property(TARGET benchmark_test PROPERTY CXX_STANDARD 17)

add_executable(test_matrix_transpose app/test_matrix_transpose.cpp)
target_link_libraries(test_matrix_transpose Catch2::Catch2)
target_include_directories(test_matrix_transpose PUBLIC include)
target_compile_options(test_matrix_transpose PUBLIC "-Wall")
set_property(TARGET test_matrix_transpose PROPERTY CXX_STANDARD 17)

add_executable(test_matrix_multiply app/test_matrix_multiply.cpp)
target_link_libraries(test_matrix_multiply Catch2::Catch2)
target_include_directories(test_matrix_multiply PUBLIC include)
target_compile_options(test_matrix_multiply PUBLIC "-Wall")
set_property(TARGET test_matrix_multiply PROPERTY CXX_STANDARD 17)

add_executable(test_fft app/test_fft.cpp)
target_link_libraries(test_fft Catch2::Catch2)
target_include_directories(test_fft PUBLIC include)
target_compile_options(test_fft PUBLIC "-Wall")
set_property(TARGET test_fft PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_matrix_transpose
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
