cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED PATHS /opt/sde/packages/catch-v2.7.2/lib64/cmake/Catch2)

project(lcg LANGUAGES CXX)

option(ENABLE_COVERAGE "Enable coverage" false)

if(ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type." FORCE)
  include(CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

add_library(ra lib/random.cpp)
target_include_directories(ra PUBLIC include)

add_executable(ra_test app/test_random.cpp) 
target_link_libraries(ra_test ra)
target_compile_options(ra_test PUBLIC "-Wall")

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE ra_test
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
