#define CATCH_CONFIG_MAIN

#include "ra/cexpr_math.hpp"

#include <catch2/catch.hpp>

using namespace ra::cexpr_math;


TEST_CASE("Functions without definition specification") {
  SECTION("Check constant.") {
    constexpr float pie = pi<float>;
    REQUIRE(pie == Approx(3.1415926));
  }

  SECTION("Round towards zero") {
    constexpr long long n = trunc<float, long long>(21873);
    constexpr int o = trunc<float>(272.56);
    constexpr int p = trunc<float>(0.1);
    constexpr int q = trunc<float>(-26.8);
    constexpr int r = trunc<float>(-2.11);
    REQUIRE(n == 21873);
    REQUIRE(o == 272);
    REQUIRE(p == 0);
    REQUIRE(q == -26);
    REQUIRE(r == -2);
  }

  SECTION("Absolute value") {
    constexpr float r = 3.1234382318231;
    constexpr float q = abs<float>(r);
    constexpr float s = abs<float>(-r);
    REQUIRE(q == Approx(r));
    REQUIRE(s == Approx(r));
  }

  SECTION("Square.") {
    constexpr float r = sqr<float>(-3.0);
    constexpr float s = sqr<float>(0.0);
    constexpr float t = sqr<float>(2.5);
    REQUIRE(r == Approx(9.0));
    REQUIRE(s == Approx(0.0));
    REQUIRE(t == Approx(6.25));
  }

  SECTION("Modulo") {
    constexpr float r = mod<float>(5, 2);
    constexpr float s = mod<float>(-13, 5);
    constexpr float t = mod<float>(17, -6);
    REQUIRE(r == Approx(1));
    REQUIRE(s == Approx(-3));
    REQUIRE(t == Approx(5));
    REQUIRE_THROWS(mod<float>(2, 0));
  }
}

TEST_CASE("Trigonometric functions.") {
  SECTION("Sine.") {
    constexpr float r = sin<float>(2 * pi<float>);
    constexpr float s = sin<float>(0);
    constexpr float t = sin<float>(pi<float> / 4.0);
    constexpr float u = sin<float>(pi<float> / -4.0);
    constexpr float v = sin<float>(7.0 * pi<float> / 3.0);
    constexpr float x = sin<float>(0.0000001);
    REQUIRE(r == Approx(0));
    REQUIRE(s == Approx(0));
    REQUIRE(t == Approx(0.70710678));
    REQUIRE(u == -t);
    REQUIRE(v == Approx(0.86602));
    REQUIRE(x == Approx(0.0000001));
  }

  SECTION("Cosine.") {
    constexpr float r = cos<float>(2 * pi<float>);
    constexpr float s = cos<float>(0);
    constexpr float t = cos<float>(9.0 * pi<float> / 4.0);
    constexpr float u = cos<float>(pi<float> * -0.5 + 0.0000001);
    REQUIRE(r == Approx(1));
    REQUIRE(s == Approx(1));
    REQUIRE(t == Approx(0.70710678));
    REQUIRE(u < 0.0001);
  }

  SECTION("Tangents.") {
    constexpr float r = tan<float>(2 * pi<float>);
    constexpr float s = tan<float>(18);
    constexpr float t = tan<float>(0);
    REQUIRE(r == Approx(0));
    REQUIRE(s == Approx(-1.13731));
    REQUIRE(t == Approx(0));
  }
}

TEST_CASE("Square root") {
  constexpr float r = sqrt<float>(2);
  constexpr float s = sqrt<float>(0);
  constexpr float t = sqrt<float>(100);
  REQUIRE(r == Approx(1.41421));
  REQUIRE(s == Approx(0));
  REQUIRE(t == Approx(10));
  REQUIRE_THROWS(sqrt<float>(-1));
}
