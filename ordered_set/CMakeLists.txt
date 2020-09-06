cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED PATHS /opt/sde/packages/catch-v2.7.2/lib64/cmake/Catch2)

project(nonintrusive_set LANGUAGES CXX)

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

add_executable(test_set app/test_sv_set.cpp)

target_include_directories(test_set PUBLIC include)
target_compile_options(test_set PUBLIC "-Wall")
set_property(TARGET test_set PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_set
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
