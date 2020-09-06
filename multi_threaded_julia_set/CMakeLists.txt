cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Boost REQUIRED)
find_package(benchmark REQUIRED)

project(julia_set LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_GCC "Use g++ or clang++." false)
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

add_library(thread_pool lib/thread_pool.cpp)
target_include_directories(thread_pool PUBLIC include)
target_compile_options(thread_pool PUBLIC "-Wall")
set_property(TARGET thread_pool PROPERTY CXX_STANDARD 17)

add_executable(test_julia_set app/test_julia_set.cpp)
target_link_libraries(test_julia_set thread_pool)
# This is a little dirty. I should create a julia_set library that links
# to the Boost include dir and then link the test program with that library.
target_include_directories(test_julia_set PUBLIC  ${Boost_INCLUDE_DIRS})
target_compile_options(test_julia_set PUBLIC "-Wall")
set_property(TARGET test_julia_set PROPERTY CXX_STANDARD 17)

add_executable(benchmark_julia_set app/benchmark.cpp)
target_link_libraries(benchmark_julia_set thread_pool benchmark::benchmark)
target_include_directories(benchmark_julia_set PUBLIC  ${Boost_INCLUDE_DIRS})
set_property(TARGET benchmark_julia_set PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_julia_set
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
