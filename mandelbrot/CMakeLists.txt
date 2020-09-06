cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED PATHS /opt/sde/packages/catch-v2.7.2/lib64/cmake/Catch2)

project(cexpr_mandelbrot LANGUAGES CXX)

option(USE_GCC "Use g++ or clang++." true)
if(USE_GCC)
  set(CMAKE_CXX_COMPILER "/usr/bin/g++")
  set(EXTRA_COMPILE_FLAGS "-fconstexpr-loop-limit=10000")
else()
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(EXTRA_COMPILE_FLAGS "-fconstexpr-steps=100000000")
endif()
set_source_files_properties(app/test_mandelbrot.cpp PROPERTIES COMPILE_FLAGS
  ${EXTRA_COMPILE_FLAGS})

option(ENABLE_COVERAGE "Enable coverage" false)
option(ENABLE_DEBUG "Enable debug." false)

if(ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type." FORCE)
  include(CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

add_executable(test_mbrot app/test_mandelbrot.cpp)

target_include_directories(test_mbrot PUBLIC include)
target_compile_options(test_mbrot PUBLIC "-Wall")
set_property(TARGET test_mbrot PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_mbrot
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
