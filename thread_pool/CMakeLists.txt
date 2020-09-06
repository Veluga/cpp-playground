cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

find_package(Catch2 REQUIRED)

project(thread_pool LANGUAGES CXX)

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

add_executable(test_thread_pool app/test_thread_pool.cpp)
target_link_libraries(test_thread_pool Catch2::Catch2 thread_pool)
target_compile_options(test_thread_pool PUBLIC "-Wall")
set_property(TARGET test_thread_pool PROPERTY CXX_STANDARD 17)

if(ENABLE_DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ENABLE_COVERAGE)
  setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE test_thread_pool
    LCOV_ARGS -rc lcov_branch_coverage=1
    GENHTML_ARGS --branch-coverage)
endif()
