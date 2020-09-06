#include "ra/thread_pool.hpp"

#include <iostream>

using namespace ra::concurrency;

thread_pool::thread_pool()
    : is_shutdown_(false), num_threads_(std::max<unsigned int>(
                               2, std::thread::hardware_concurrency())),
      q_(32) {
  init_threads();
}

thread_pool::thread_pool(std::size_t num_threads)
    : is_shutdown_(false), num_threads_(num_threads), q_(32) {
  init_threads();
}

thread_pool::~thread_pool() {
  if (!is_shutdown()) {
    shutdown();
  }
}

void thread_pool::schedule(std::function<void()> &&func) {
	std::unique_lock l(m_);
	if (is_shutdown()) {
		return;
	}
	q_.push(std::move(func));
}

void thread_pool::shutdown() {
	std::unique_lock l(m_);
	if (is_shutdown()) {
		return;
	}
	is_shutdown_ = true;
	q_.close();
	for (auto &t : threads_) {
		t.join();
	}
}

thread_pool::size_type thread_pool::size() const { return num_threads_; }

bool thread_pool::is_shutdown() const { return is_shutdown_; }

void thread_pool::init_threads() {
  threads_.reserve(size());
  for (int i = 0; i < size(); ++i) {
    threads_.emplace_back([this]() {
      while (true) {
        std::function<void()> job;
        queue<std::function<void()>>::status s = q_.pop(job);
        if (s == queue<std::function<void()>>::status::success) {
          job();
        } else {
          return;
        }
      }
    });
  }
}
