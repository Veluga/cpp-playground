#define CATCH_CONFIG_MAIN

#include "ra/random.hpp"

#include <catch2/catch.hpp>
#include <random>
#include <ostream>

using lcg = ra::random::linear_congruential_generator;

TEST_CASE("int_type has size at least 64 bits.") {
  REQUIRE(sizeof(lcg::int_type) >= 8);
}

TEST_CASE("Default seed should always be 1u.") {
  REQUIRE(lcg::default_seed() == 1u);
}

TEST_CASE("Equality properties of LCGs hold.") {
  lcg r1 = lcg(48271, 0, 2147483647);
  lcg r2 = lcg(48271, 0, 2147483647);

  SECTION("Equality check.") {
    REQUIRE(r1 == r2);
    REQUIRE(!(r1 != r2));
    r1();
    r2();
    REQUIRE(r1 == r2);
    REQUIRE(!(r1 != r2));

    r1.seed(14);
    r2.seed(14); 
    
    REQUIRE(r1 == r2);
    REQUIRE(!(r1 != r2));
    r1();
    r2();
    REQUIRE(r1 == r2);
    REQUIRE(!(r1 != r2));

    r1.discard(137);
    r2.discard(138);
    REQUIRE(r1 != r2);
  }
}

TEST_CASE("RN Generation works as with STL LCG.") {
  lcg g = lcg(48271, 0, 2147483647);
  std::minstd_rand t = std::minstd_rand();

  REQUIRE(g() == t());
  
  g.discard(1024);
  t.discard(1024);

  REQUIRE(g() == t());

  g.seed(1337);
  t.seed(1337);
  REQUIRE(g() == t());

  g.seed(2147483647);
  t.seed(2147483647);
  REQUIRE(g() == t());
}

TEST_CASE("Writing to ostream.") {
  lcg g = lcg(48271, 0, 2147483647);
  std::ostringstream os = std::ostringstream();

  os << g;
  REQUIRE(os);
  REQUIRE(os.str() == "48271 0 2147483647 1");

  g.seed(147);
  os = std::ostringstream();
  os << g;
  REQUIRE(os.str() == "48271 0 2147483647 147");
}

TEST_CASE("LCG Getters.") {
  lcg g = lcg(48271, 0, 2147483647);
  REQUIRE(g.multiplier() == 48271);
  REQUIRE(g.increment() == 0);
  REQUIRE(g.modulus() == 2147483647);
  REQUIRE(g.min() == 1);
  REQUIRE(g.max() == 2147483646);

  g = lcg(48271, 1, 2147483647);

  REQUIRE(g.min() == 0);
}

TEST_CASE("Instantiation.") {
  lcg g = lcg(48271, 0, 2147483647);
  lcg r = lcg(g);
  REQUIRE(r == g);
  r = g;
  REQUIRE(r == g);
  r = lcg(lcg(48271, 0, 2147483647));
  REQUIRE(r == g);
}

TEST_CASE("Const correctness") {
  const lcg g = lcg(48271, 0, 2147483647);
  g.min();
  g.max();
  g.multiplier();
  g.increment();
  g.modulus();
}
