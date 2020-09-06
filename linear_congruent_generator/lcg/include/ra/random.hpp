#ifndef RANDOM_LCG_H
#define RANDOM_LCG_H

#include <ostream>
#include <type_traits>

// Forward declaration

namespace ra {
namespace random {
class linear_congruential_generator;
}
} // namespace ra

std::ostream &operator<<(std::ostream &,
                         const ra::random::linear_congruential_generator &);

namespace ra {
namespace random {
class linear_congruential_generator {
public:
  using int_type = std::conditional<sizeof(unsigned long) >= 8, unsigned long,
                                    unsigned long long>::type;

  static int_type default_seed();
  linear_congruential_generator(int_type, int_type, int_type,
                                int_type = default_seed());
  linear_congruential_generator() = delete;
  int_type multiplier() const;
  int_type increment() const;
  int_type modulus() const;
  void seed(int_type);
  int_type operator()();
  void discard(unsigned long long);
  int_type min() const;
  int_type max() const;
  bool operator==(const linear_congruential_generator &) const;
  bool operator!=(const linear_congruential_generator &) const;
  friend std::ostream & ::operator<<(std::ostream &,
                                     const linear_congruential_generator &);

private:
  int_type a;
  int_type c;
  int_type m;
  int_type current;
};
} // namespace random
} // namespace ra

#endif
