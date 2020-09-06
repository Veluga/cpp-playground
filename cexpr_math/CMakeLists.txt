cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED PATHS /opt/sde/packages/catch-v2.7.2/lib64/cmake/Catch2)

project(cexpr_math LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

option(ENABLE_COVERAGE "Enable coverage" false)
option(ENABLE_DEBUG "Enable debug." false)

if(ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type." FORCE)
  include(CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

add_executable(test_math app/test_cexpr_math.cpp)
target_include_directories(test_math PUBLIC include)
target_include_directories(test_math PUBLIC /opt/sde/packages/boost-1.69.0/include)

target_compile_options(test_math PUBLIC "-Wall")
set_property(TARGET test_math PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_math
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
