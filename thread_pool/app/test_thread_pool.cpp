#define CATCH_CONFIG_MAIN

#include "ra/thread_pool.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

using namespace ra::concurrency;

TEST_CASE("Initialization.") {
  SECTION("Default construction.") {
    thread_pool tp;
    REQUIRE(tp.size() ==
            std::max<unsigned int>(std::thread::hardware_concurrency(), 2));
  }

  SECTION("Size construction.") {
    thread_pool tp2(12);
    REQUIRE(tp2.size() == 12);
  }
}

TEST_CASE("Scheduling.") {
  SECTION("Single threaded.") {
    thread_pool tp;
    for (int i = 0; i < 1024; ++i) {
      std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(5));
      tp.schedule([i]() {
        std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(5));
        int j = i * i + i;
      });
    }
    tp.shutdown();
		REQUIRE(true);
  }

  SECTION("Multi threaded.") {
    thread_pool tp;
    auto schedule_op = [&tp]() {
      for (int i = 0; i < 1024; ++i) {
        std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(5));
        tp.schedule([i]() {
          std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(5));
          int j = i * i + i;
        });
      }
      tp.shutdown();
    };

		std::thread t1(schedule_op), t2(schedule_op), t3(schedule_op);
		t1.join();
		t2.join();
		t3.join();
		REQUIRE(true);
  }
}
