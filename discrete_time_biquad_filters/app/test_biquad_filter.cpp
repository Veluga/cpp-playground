#define CATCH_CONFIG_MAIN

#include "ra/biquad_filter.hpp"

#include <catch2/catch.hpp>

using namespace ra::biquad;

TEST_CASE("Lowpass filter.") {
  constexpr biquad_filter_coefs<float> c = lowpass<float>(0.45351, 0.7071);
  REQUIRE(c.a0 == Approx(0.25137).epsilon(0.01));
  REQUIRE(c.a1 == Approx(0.50275).epsilon(0.01));
  REQUIRE(c.a2 == Approx(0.25137).epsilon(0.01));
  REQUIRE(c.b0 == Approx(1).epsilon(0.01));
  REQUIRE(c.b1 == Approx(-0.17124).epsilon(0.01));
  REQUIRE(c.b2 == Approx(0.17675).epsilon(0.01));
}

TEST_CASE("Highpass filter.") {
  constexpr biquad_filter_coefs<float> c = highpass<float>(0.45351, 0.7071);
  REQUIRE(c.a0 == Approx(0.33699).epsilon(0.01));
  REQUIRE(c.a1 == Approx(-0.67399).epsilon(0.01));
  REQUIRE(c.a2 == Approx(0.33699).epsilon(0.01));
  REQUIRE(c.b0 == Approx(1).epsilon(0.01));
  REQUIRE(c.b1 == Approx(-0.17124).epsilon(0.01));
  REQUIRE(c.b2 == Approx(0.17675).epsilon(0.01));
}

TEST_CASE("Bandpass filter.") {
  constexpr biquad_filter_coefs<float> c = bandpass<float>(0.45351, 0.7071);
  REQUIRE(c.a0 == Approx(0.41162).epsilon(0.01));
  REQUIRE(c.a1 == Approx(0).epsilon(0.01));
  REQUIRE(c.a2 == Approx(-0.41162).epsilon(0.01));
  REQUIRE(c.b0 == Approx(1).epsilon(0.01));
  REQUIRE(c.b1 == Approx(-0.17124).epsilon(0.01));
  REQUIRE(c.b2 == Approx(0.17675).epsilon(0.01));
}

TEST_CASE("Low-frequency shelving boost filter.") {
  constexpr biquad_filter_coefs<float> c = lowshelf_boost<float>(0.45351, 1.99526);
  REQUIRE(c.a0 == Approx(1.41999).epsilon(0.01));
  REQUIRE(c.a1 == Approx(0.32913).epsilon(0.01));
  REQUIRE(c.a2 == Approx(0.25714).epsilon(0.01));
  REQUIRE(c.b0 == Approx(1).epsilon(0.01));
  REQUIRE(c.b1 == Approx(-0.17124).epsilon(0.01));
  REQUIRE(c.b2 == Approx(0.17675).epsilon(0.01));
}


TEST_CASE("Low-frequency shelving cut filter.") {
  constexpr biquad_filter_coefs<float> c = lowshelf_cut<float>(0.45351, 1.99526);
  REQUIRE(c.a0 == Approx(0.70422).epsilon(0.01));
  REQUIRE(c.a1 == Approx(-0.12059).epsilon(0.01));
  REQUIRE(c.a2 == Approx(0.12448).epsilon(0.01));
  REQUIRE(c.b0 == Approx(1).epsilon(0.01));
  REQUIRE(c.b1 == Approx(0.23178).epsilon(0.01));
  REQUIRE(c.b2 == Approx(0.18108).epsilon(0.01));
}
