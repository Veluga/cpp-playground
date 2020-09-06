#include "ra/random.hpp"

using lcg = ra::random::linear_congruential_generator;

lcg::int_type lcg::default_seed() { return 1u; }

lcg::linear_congruential_generator(lcg::int_type a, lcg::int_type c,
                                   lcg::int_type m, lcg::int_type s)
    : a(a), c(c), m(m) {
  seed(s);
}

void lcg::seed(lcg::int_type s) { current = c % m == 0 && s % m == 0 ? 1 : s; }

lcg::int_type lcg::multiplier() const { return a; }

lcg::int_type lcg::increment() const { return c; }

lcg::int_type lcg::modulus() const { return m; }

lcg::int_type lcg::operator()() {
  current = (a * current + c) % m;
  return current;
}

void lcg::discard(unsigned long long n) {
  for (; n > 0ULL; --n) {
    (*this)();
  }
}

lcg::int_type lcg::min() const { return c == 0 ? 1u : 0u; }

lcg::int_type lcg::max() const { return m - 1; }

bool lcg::operator==(const lcg &g) const {
  return g.current == current && g.a == a && g.c == c && g.m == m;
}

bool lcg::operator!=(const lcg &g) const { return !((*this) == g); }

std::ostream &operator<<(std::ostream &os,
                         const ra::random::linear_congruential_generator &g) {
  os << g.a << " " << g.c << " " << g.m << " " << g.current;
  return os;
}
