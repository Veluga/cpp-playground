#define CATCH_CONFIG_MAIN

#include "ra/queue.hpp"

#include <catch2/catch.hpp>
#include <functional>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

using namespace ra::concurrency;

/* Helper functions */

int get_random_int() {
    static std::random_device rd;  
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dist(0, 1000);
		return dist(gen);
}

/* Test cases */

TEST_CASE("Add elements to queue.") {
  for (int i = 0; i < 10'000; ++i) {
    std::vector<std::thread> v;
    queue<int> q(20);
    for (int j = 0; j < 20; ++j) {
      v.emplace_back([&q](int x) { q.push(std::move(x)); }, j);
    }
    for (int j = 0; j < 20; ++j) {
      v[j].join();
    }
    REQUIRE(q.is_full());
  }
}

TEST_CASE("Remove elements from queue.") {
  queue<double> q(20);
  for (int i = 0; i < 10'000; ++i) {
    for (int j = 0; j < 20; ++j) {
      q.push(j);
    }
    std::vector<std::thread> v;
    for (int j = 0; j < 20; ++j) {
      v.emplace_back([&q]() {
        double d;
        q.pop(d);
      });
    }
    for (int j = 0; j < 20; ++j) {
      v[j].join();
    }
    REQUIRE(q.is_empty());
  }
}

TEST_CASE("Fuzzy testing.") {
  queue<int> q(100);
  for (int i = 0; i < 100; ++i) {
		q.clear();
    std::vector<std::thread> v;
    for (int j = 0; j < 1000; ++j) {
			if (get_random_int() == 420) {
				// Randomly close queue
				q.close();
			}
			v.emplace_back([&q]() { q.push(get_random_int()); });
			v.emplace_back([&q]() { int i; q.pop(i); });
    }
    for (int j = 0; j < 2000; ++j) {
      v[j].join();
    }
		REQUIRE(true);
  }
}
