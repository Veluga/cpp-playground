#include "ra/queue.hpp"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>

namespace ra::concurrency {
class thread_pool {
public:
  using size_type = std::size_t;

  thread_pool();
  thread_pool(std::size_t num_threads);

  thread_pool(const thread_pool &) = delete;
  thread_pool &operator=(const thread_pool &) = delete;
  thread_pool(thread_pool &&) = delete;
  thread_pool &operator=(thread_pool &&) = delete;

  ~thread_pool();

  size_type size() const;

  void schedule(std::function<void()> &&func);

  void shutdown();

  bool is_shutdown() const;

private:
  bool is_shutdown_;
  int num_threads_;
  std::mutex m_;
  std::condition_variable cv_;
	std::vector<std::thread> threads_;
	queue<std::function<void()>> q_;

	void init_threads();
};
} // namespace ra::concurrency
