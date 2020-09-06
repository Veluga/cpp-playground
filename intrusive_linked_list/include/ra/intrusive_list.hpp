#ifndef RA_INTRUSIVE_LIST
#define RA_INTRUSIVE_LIST

#include "iterator_facade.hpp"
#include "parent_from_member.hpp"

#include <stdexcept>
#include <utility>

using namespace ra::util;

// Forward declarations
namespace ra::intrusive {
class list_hook;
template <class T, list_hook T::*Hook> class list;
} // namespace ra::intrusive
// End of forward declarations

namespace ra::intrusive {
class list_hook {
public:
  list_hook() = default;
  list_hook(const list_hook &) = default;
  list_hook(list_hook &&) = default;
  list_hook &operator=(const list_hook &) = default;
  list_hook &operator=(list_hook &&);
  ~list_hook() = default;

private:
  template <class T, list_hook T::*> friend class list;
  template <class Value, list_hook Value::*> friend class list_iterator;
  list_hook *next_;
  list_hook *prev_;
};

template <class Value, list_hook Value::*Hook>
class list_iterator
    : public bidirectional_iterator_facade<list_iterator<Value, Hook>, Value> {
public:
  list_iterator() = default;
  list_iterator(list_hook &node) : node_(&node){};
  list_iterator(const list_iterator &) = default;
  list_iterator &operator=(const list_iterator &) = default;

private:
  template <class T, list_hook T::*> friend class list;
  list_hook *node_;
  list_iterator &increment() {
    node_ = node_->next_;
    return *this;
  }
  list_iterator &decrement() {
    node_ = node_->prev_;
    return *this;
  }
  bool equal(const list_iterator &other) const { return node_ == other.node_; }
  Value &dereference() const {
    return *parent_from_member<Value, list_hook>(node_, Hook);
  }
};

template <class T, list_hook T::*Hook> class list {
public:
  static constexpr list_hook T::*hook_ptr = Hook;
  inline static std::ptrdiff_t offset_to_member =
      offset_from_pointer_to_member<T, list_hook>(hook_ptr);

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = list_iterator<value_type, hook_ptr>;
  using const_iterator = list_iterator<const value_type, hook_ptr>;
  using size_type = std::size_t;

  list() : size_(0) { sentinel.next_ = sentinel.prev_ = &sentinel; };
  ~list() { clear(); };
  list(list &&other) {
    clear();
    swap(other);
  };
  list &operator=(list &&other) {
    if (this != &other) {
      if (size_ > 0) {
        clear();
      }
      swap(other);
    }
    return *this;
  };
  list(const list &) = delete;
  list &operator=(const list &) = delete;

  void swap(list &x) {
    std::swap(size_, x.size_);
    list_hook tmp = sentinel;
    sentinel = x.sentinel;
    x.sentinel = tmp;
  };
  size_type size() const { return size_; };
  iterator insert(iterator pos, reference value) {
    list_hook *value_hook = static_cast<list_hook *>(static_cast<void *>(
        static_cast<char *>(static_cast<void *>(&value)) + offset_to_member));
    pos.node_->prev_->next_ = value_hook;
    value_hook->prev_ = pos.node_->prev_;
    value_hook->next_ = pos.node_;
    pos.node_->prev_ = value_hook;
    ++size_;
    return iterator(*value_hook);
  };
  iterator erase(iterator pos) {
    pos.node_->prev_->next_ = pos.node_->next_;
    pos.node_->next_->prev_ = pos.node_->prev_;
    iterator it(*pos.node_->next_);
    pos.node_->next_ = nullptr;
    pos.node_->prev_ = nullptr;
    --size_;
    return it;
  };

  void push_back(reference x) { insert(end(), x); };
  void pop_back() {
    if (!size()) {
      throw std::runtime_error("Can not pop empty list.");
    }
    erase(iterator(*sentinel.prev_));
  };
  reference back() {
    if (!size()) {
      throw std::runtime_error("Can not retrieve last element of empty list");
    }
    return *iterator(*sentinel.prev_);
  };
  const_reference back() const {
    if (!size()) {
      throw std::runtime_error("Can not retrieve last element of empty list.");
    }
    return *const_iterator(*sentinel.prev_);
  };
  void clear() {
    sentinel.next_ = sentinel.prev_ = &sentinel;
    size_ = 0;
  };

  const_iterator begin() const { return const_iterator(*sentinel.next_); };
  iterator begin() { return iterator(*sentinel.next_); };
  const_iterator end() const { return const_iterator(sentinel); };
  iterator end() { return iterator(sentinel); };

private:
  list_hook sentinel;
  std::size_t size_;
};
} // namespace ra::intrusive

#endif
