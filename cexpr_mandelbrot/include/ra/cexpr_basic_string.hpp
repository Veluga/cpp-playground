#include <cstring>
#include <stdexcept>
#include <cmath>

namespace ra::cexpr {
template <class T, std::size_t M> class cexpr_basic_string {
public:
  using size_type = std::size_t;
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using iterator = pointer;
  using const_iterator = const_pointer;

  constexpr cexpr_basic_string() : s_{} {};
  constexpr cexpr_basic_string(const cexpr_basic_string &) = default;
  constexpr cexpr_basic_string &operator=(const cexpr_basic_string &) = default;
  ~cexpr_basic_string() = default;

  constexpr cexpr_basic_string(const_pointer s) : s_{} {
    size_t i = 0;
    for (; s[i]; ++i) {
      if (i > M) {
        throw std::runtime_error("String length exceeds capacity.");
      }
      s_[i] = s[i];
    }
    size_ = i;
  };
  constexpr cexpr_basic_string(const_iterator first, const_iterator last)
      : s_{} {
    size_t len = static_cast<size_t>(last - first);
    if (len > M) {
      throw std::runtime_error("String length exceeds capacity.");
    } else {
      size_ = len;
    }
    for (int i = 0; i < (last - first); ++i) {
      s_[i] = *(first + i);
    }
  };
  static constexpr size_type max_size() { return M; };
  constexpr size_type capacity() const { return M; };
  constexpr size_type size() const { return size_; };
  pointer data() { return s_; };
  const_pointer data() const { return s_; };
  constexpr iterator begin() { return s_; };
  constexpr const_iterator begin() const { return s_; };
  constexpr iterator end() {
    // End points one past the last element
    return s_ + size_;
  };
  constexpr const_iterator end() const {
    // End points one past the last element
    return s_ + size_;
  };
  constexpr reference operator[](size_type i) { return s_[i]; };
  constexpr const_reference operator[](size_type i) const { return s_[i]; };
  constexpr void push_back(const_reference x) {
    if (size_ == M) {
      throw std::runtime_error("Adding character exceeds capacity.");
    }
    s_[size_++] = x;
  };
  constexpr void pop_back() {
    if (size_ == 0) {
      throw std::runtime_error("Can not pop empty string.");
    }
    s_[--size_] = 0;
  };
  constexpr cexpr_basic_string &append(const_pointer s) {
    int len = 0;
    for (; s[len]; ++len)
      ;
    if (size_ + len > M) {
      throw std::runtime_error("Appending to string would exceed capacity.");
    }
    for (int i = 0; s[i]; ++i) {
      s_[size_++] = s[i];
    }
    return *this;
  };

  template <size_type OtherM>
  constexpr cexpr_basic_string &
  append(const cexpr_basic_string<value_type, OtherM> &other) {
    if (size_ + other.size() > M) {
      throw std::runtime_error("Appending to string would exceed capacity.");
    }
    for (const_iterator i = other.begin(); i < other.end(); ++i) {
      s_[size_++] = *i;
    }
    return *this;
  };
  constexpr void clear() {
    for (size_t i = 0; i < size_; ++i) {
      s_[i] = 0;
    }
    size_ = 0;
  };

private:
  value_type s_[M + 1];
  size_t size_ = 0;
};

template <std::size_t M> using cexpr_string = cexpr_basic_string<char, M>;

// Constexpr helper since std::pow is not constexpr
template <class T> constexpr T pow(int b, int exp) {
  // Only positive base / exponent will be used (avoids case handling)
  unsigned int res = 1;
  for (; exp > 0; --exp) {
    res *= b;
  }
  return static_cast<T>(res);
}

constexpr std::size_t to_string(std::size_t n, char *buffer, std::size_t size,
                                char **end) {
  int exp = 0;
  std::size_t len = 0;

  for (; n / pow<size_t>(10, exp + 1); ++exp);

  while (exp >= 0) {
    if (len + 1 >= size) {
      throw std::runtime_error("Buffer capacity insufficient for holding result.");
    }
    *(buffer + len++) = '0' + n / pow<size_t>(10, exp);
    n %= pow<size_t>(10, exp--);
  }

  *end = buffer + len;
  **end = 0;

  return len;
};

} // namespace ra::cexpr
