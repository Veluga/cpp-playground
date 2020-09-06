#ifndef RA_SV_SET
#define RA_SV_SET

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

namespace ra::container {
template <class Key, class Compare = std::less<Key>> class sv_set {
public:
  struct ordered_and_unique_range {};

  using value_type = Key;
  using key_type = Key;
  using key_compare = Compare;
  using size_type = std::size_t;
  using iterator = Key *;
  using const_iterator = Key * const;

  sv_set() noexcept(std::is_nothrow_default_constructible_v<key_compare>)
      : start_(nullptr), finish_(nullptr), end_(nullptr){};
  template <class InputIterator>
  sv_set(ordered_and_unique_range, InputIterator first, std::size_t n) {
    start_ = static_cast<value_type *>(::operator new(n * sizeof(value_type)));
    try {
      finish_ = std::uninitialized_copy_n(first, n, start_);
      end_ = start_ + n;
    } catch (...) {
      ::operator delete(start_);
      throw;
    }
  };
  sv_set(sv_set &&other) noexcept(
      std::is_nothrow_move_constructible_v<key_compare>)
      : cmp{std::move(other.cmp)} {
    start_ = other.start_;
    other.start_ = nullptr;
    finish_ = other.finish_;
    other.finish_ = nullptr;
    end_ = other.end_;
    other.end_ = nullptr;
  };
  sv_set &operator=(sv_set &&other) noexcept(
      std::is_nothrow_move_assignable_v<key_compare>) {
    // To provide a strong exception safety guarantee, we move the Compare
    // instance first since we can not be certain that it is nothrow move
    // assignable.
    cmp = std::move(other.cmp);

    clear();
    ::operator delete(start_);

    start_ = other.start_;
    other.start_ = nullptr;
    finish_ = other.finish_;
    other.finish_ = nullptr;
    end_ = other.end_;
    other.end_ = nullptr;

    return *this;
  };
  sv_set(const sv_set &other) : cmp{other.cmp} {
    start_ = static_cast<value_type *>(
        ::operator new(other.size() * sizeof(value_type)));
    try {
      finish_ = std::uninitialized_copy(other.begin(), other.finish_, start_);
      end_ = finish_;
    } catch (...) {
      ::operator delete(start_);
      throw;
    }
  };
  sv_set &operator=(const sv_set &other) {
    if (this != &other) {
      sv_set tmp(other);
      swap(tmp);
    }
    return *this;
  };
  ~sv_set() {
    clear();
    ::operator delete(start_);
  };

  key_compare key_comp() const { return cmp; };
  const_iterator begin() const noexcept { return start_; };
  iterator begin() noexcept { return start_; };
  const_iterator end() const noexcept { return finish_; };
  iterator end() noexcept { return finish_; };

  size_type size() const noexcept { return finish_ - start_; };
  size_type capacity() const noexcept { return end_ - start_; };
  void reserve(size_type n) {
    if (n <= capacity()) {
      return;
    }
    grow(n);
  };
  void shrink_to_fit() {
    if (capacity() > size()) {
      sv_set tmp(ordered_and_unique_range(), start_, size());
      swap(tmp);
    }
  };

  std::pair<iterator, bool> insert(const key_type &x) {
    iterator idx = find(x);
    if (idx != finish_) {
      return std::pair<iterator, bool>(idx, false);
    }
    if (size() == capacity()) {
      grow(capacity()? growth_factor * capacity() : 1);
    }
    idx = std::lower_bound<iterator, value_type, key_compare>(start_, finish_, x, cmp);
    for (iterator current = finish_; current > idx; --current) {
      *current = *(current - 1);
      (current - 1)->~value_type();
    }
    *idx = x;
    ++finish_;
    return std::pair<iterator, bool>(idx, true);
  };
  iterator erase(const_iterator pos) {
    if (pos >= finish_) {
      return finish_;
    }
    pos->~value_type();
    for(iterator current = pos; current < finish_ - 1; ++current) {
      *current = *(current + 1);
      (current + 1)->~value_type();
    }
    --finish_;
    return pos;
  };
  void swap(sv_set &x) noexcept(std::is_nothrow_swappable_v<key_compare>) {
    // To provide a strong exception safety guarantee, we swap the Compare
    // instance first since we can not be certain that it is nothrow move
    // assignable.
    key_compare tmp = std::move(cmp);
    cmp = std::move(x.cmp);
    x.cmp = std::move(tmp);
    // Assumption: iterator is swappable
    std::swap(start_, x.start_);
    std::swap(finish_, x.finish_);
    std::swap(end_, x.end_);
  };
  void clear() noexcept {
    if (size()) {
      std::destroy(start_, finish_);
      finish_ = start_;
    }
  };

  iterator find(const key_type &k) {
    return const_cast<iterator>(static_cast<const sv_set&>(*this).find(k));
  };
  const_iterator find(const key_type &k) const {
    if (!size()) {
      return finish_;
    }
    iterator lo = std::lower_bound<iterator, value_type, key_compare>(
        start_, finish_, k, cmp);
    iterator hi = std::upper_bound<iterator, value_type, key_compare>(
        start_, finish_, k, cmp);
    return lo == hi - 1 ? lo : finish_;
  };

private:
  void grow(size_type n) {
    if (n <= capacity()) {
      return;
    }
    iterator new_start =
        static_cast<iterator>(::operator new(n * sizeof(value_type)));
    try {
      iterator new_finish = std::uninitialized_copy_n(start_, size(), new_start);
      clear();
      ::operator delete(start_);
      start_ = new_start;
      finish_ = new_finish;
      end_ = start_ + n;
    } catch (...) {
      ::operator delete(new_start);
      throw;
    }
  };
  iterator start_;
  iterator finish_;
  iterator end_;
  Compare cmp;
  constexpr static size_type growth_factor = 2;
};
} // namespace ra::container

#endif
