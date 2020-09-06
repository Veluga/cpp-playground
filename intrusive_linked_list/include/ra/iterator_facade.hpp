#ifndef LIB_ITERATOR_FACADE
#define LIB_ITERATOR_FACADE

#include <type_traits>

template <class Derived, class Value>
class bidirectional_iterator_facade {
  public:
    bidirectional_iterator_facade() = default;
    bidirectional_iterator_facade(const bidirectional_iterator_facade&) = default;
    bidirectional_iterator_facade& operator=(const bidirectional_iterator_facade&) = default;
    ~bidirectional_iterator_facade() = default;

    Derived& operator++() {
      return increment();
    }
    Derived operator++(int) {
      Derived tmp = *this;
      ++(*this);
      return tmp;
    }
    Derived operator--() {
      return decrement();
    }
    Derived operator--(int) {
      Derived tmp = *this;
      --(*this);
      return tmp;
    }
    bool operator==(const Derived& other) const {
      return equal(other);
    }
    bool operator!=(const Derived& other) const {
      return !(*this == other);
    }
    Value& operator*() const {
      return dereference();
    }

  private:
    virtual Derived& increment() = 0;
    virtual Derived& decrement() = 0;
    virtual bool equal(const Derived&) const = 0;
    virtual Value& dereference() const = 0;
};

#endif
