#include <condition_variable>
#include <mutex>
#include <queue>

namespace ra::concurrency {
template <class T> class queue {
public:
  using value_type = T;
  using size_type = std::size_t;
  enum class status {
    success = 0,
    empty,
    full,
    closed,
  };

  queue() = delete;
  queue(size_type max_size) : max_size_(max_size), open_(true){};
  queue(const queue &) = delete;
  queue &operator=(const queue &) = delete;
  queue(queue &&) = delete;
  queue &operator=(queue &&) = delete;
  ~queue() = default;

  status push(value_type &&x) {
    std::unique_lock l(m_);
    if (is_full()) {
      full_.wait(l, [this]() { return (is_closed() || !is_full()); });
    }
		if (is_closed()) {
			l.unlock();
			full_.notify_all();
			empty_.notify_all();
			return status::closed;
		}
		q_.push(x);

		l.unlock();
		empty_.notify_all();
		return status::success;
  };
  status pop(value_type &x) {
		std::unique_lock l(m_);
		if (is_empty()) {
			empty_.wait(l, [this]() { return (is_closed() || !is_empty()); });	
		}
		if (is_closed() && is_empty()) {
			l.unlock();
			empty_.notify_all();
			full_.notify_all();
			return status::closed;
		}
		// Element will be popped of the queue afterwards, hence we can use move assignment
		x = std::move(q_.front());
		q_.pop();

		l.unlock();
		full_.notify_all();
		return status::success;
	};

  void close() {
		if (is_closed()) {
			return;
		}
		std::unique_lock l(m_);
		open_ = false;
		full_.notify_all();
		empty_.notify_all();
	};
  void clear() {
		std::unique_lock l(m_);
		std::queue<value_type> new_q;
		q_.swap(new_q);

		l.unlock();
		full_.notify_all();
	};

  bool is_full() const { return q_.size() == max_size_; };
  bool is_empty() const { return q_.empty(); };
  bool is_closed() const { return !open_; };

  size_type max_size() const;

private:
  size_type max_size_;
  bool open_;
  std::mutex m_;
  std::condition_variable full_, empty_;
  std::queue<value_type> q_;
};
} // namespace ra::concurrency
