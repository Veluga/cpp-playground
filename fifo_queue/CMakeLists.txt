cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED)

project(fifo_queue LANGUAGES CXX)

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

add_executable(test_queue app/test_queue.cpp)
target_link_libraries(test_queue Catch2::Catch2)
target_include_directories(test_queue PUBLIC include)
target_compile_options(test_queue PUBLIC "-Wall")
set_property(TARGET test_queue PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_queue
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
