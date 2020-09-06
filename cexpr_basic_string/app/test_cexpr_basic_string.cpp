#define CATCH_CONFIG_MAIN

#include "ra/cexpr_basic_string.hpp"

#include <catch2/catch.hpp>
#include <iostream>
#include <type_traits>

using ra::cexpr::cexpr_basic_string;
using ra::cexpr::cexpr_string;
using ra::cexpr::to_string;

TEST_CASE("Aliases.") {
  REQUIRE(std::is_same<cexpr_string<256>::size_type, std::size_t>::value);
  REQUIRE(std::is_same<cexpr_string<256>::value_type, char>::value);
  REQUIRE(std::is_same<cexpr_basic_string<unsigned char, 256>::value_type,
                       unsigned char>::value);
  REQUIRE(std::is_same<cexpr_string<256>::pointer, char *>::value);
  REQUIRE(std::is_same<cexpr_string<256>::const_pointer, const char *>::value);
  REQUIRE(std::is_same<cexpr_string<256>::reference, char &>::value);
  REQUIRE(
      std::is_same<cexpr_string<256>::const_reference, const char &>::value);
  REQUIRE(std::is_same<cexpr_string<256>::iterator, char *>::value);
  REQUIRE(std::is_same<cexpr_string<256>::const_iterator, const char *>::value);
}

TEST_CASE("Initialization.") {
  constexpr auto s = cexpr_string<256>();
  REQUIRE(s.data()[0] == 0);
  REQUIRE(s.size() == 0);

  constexpr auto t = cexpr_string<256>("Hello!");
  REQUIRE(std::strcmp(t.data(), "Hello!") == 0);
  REQUIRE(t.size() == 6);

  CHECK_NOTHROW(cexpr_string<0>(""));
  CHECK_NOTHROW(cexpr_string<1>("a"));
  CHECK_THROWS(cexpr_string<1>("Too long..."));

  constexpr char test_s[] = "Nintendo";
  constexpr auto u = cexpr_string<256>(test_s, test_s + 8);
  REQUIRE(std::strcmp(u.data(), test_s) == 0);
  REQUIRE(u.size() == 8);

  CHECK_THROWS(cexpr_string<4>(test_s, test_s + 8));
  CHECK_NOTHROW(cexpr_string<8>(test_s, test_s + 8));
  CHECK_NOTHROW(cexpr_string<0>(test_s, test_s));

  constexpr auto v = cexpr_string<256>(u);
  REQUIRE(std::strcmp(u.data(), v.data()) == 0);
  REQUIRE(v.size() == 8);

  constexpr auto w = v;
  REQUIRE(std::strcmp(w.data(), v.data()) == 0);
  REQUIRE(w.size() == 8);
}

TEST_CASE("Functions related to string size.") {
  SECTION("Max size.") {
    constexpr auto s = cexpr_basic_string<unsigned char, 1024>();
    REQUIRE(decltype(s)::max_size() == 1024);
    REQUIRE(cexpr_string<444>::max_size() == 444);
    REQUIRE(cexpr_string<0>::max_size() == 0);
  }

  SECTION("Capacity.") {
    constexpr auto s = cexpr_string<1024>();
    REQUIRE(s.capacity() == 1024);
    REQUIRE(cexpr_basic_string<unsigned char, 1024>().capacity() == 1024);
    REQUIRE(cexpr_string<77>().capacity() == 77);
  }

  SECTION("Size.") {
    constexpr auto s = cexpr_string<1024>("Hello, World!");
    REQUIRE(s.size() == std::strlen("Hello, World!"));
    REQUIRE(cexpr_string<17>().size() == 0);
  }
}

TEST_CASE("Iterators.") {
  SECTION("Begin iterators.") {
    constexpr auto s = cexpr_string<1024>("Dogs are cool animals.");
    REQUIRE(s.begin() == s.data());

    constexpr auto t = cexpr_string<512>();
    REQUIRE(t.begin() == t.data());

    auto u = cexpr_string<256>();
    REQUIRE(u.begin() == u.data());
  }
  SECTION("End iterators.") {
    constexpr auto s = cexpr_string<64>("Cats are less cool.");
    REQUIRE(s.end() == s.data() + s.size());

    constexpr auto t = cexpr_string<64>();
    REQUIRE(t.end() == t.data());

    auto u = cexpr_string<256>();
    REQUIRE(u.end() == u.data());
  }

  SECTION("Combined iterators.") {
    constexpr char test_string[] = "Mickey Mouse";
    constexpr auto s = cexpr_string<256>(test_string);
    for (int i = 0; i < s.end() - s.begin(); ++i) {
      REQUIRE(s[i] == test_string[i]);
    }
  }
}

TEST_CASE("Modifying string.") {
  SECTION("Push back.") {
    auto s = cexpr_string<256>("ABCDEF");
    CHECK_NOTHROW(s.push_back('G'));
    REQUIRE(std::strcmp(s.data(), "ABCDEFG") == 0);
    REQUIRE(s.size() == 7);

    auto t = cexpr_string<3>("ABC");
    CHECK_THROWS(t.push_back('D'));
  }

  SECTION("Pop back.") {
    auto s = cexpr_string<256>("ABCDEF");
    CHECK_NOTHROW(s.pop_back());
    REQUIRE(std::strcmp(s.data(), "ABCDE") == 0);
    REQUIRE(s.size() == 5);

    auto t = cexpr_string<256>();
    CHECK_THROWS(t.pop_back());
  }

  SECTION("Appending.") {
    auto s = cexpr_string<12>("ABCDEF");
    CHECK_NOTHROW(s.append("GHIJKL"));
    REQUIRE(std::strcmp(s.data(), "ABCDEFGHIJKL") == 0);
    CHECK_THROWS(s.append("M"));

    auto t = cexpr_string<2>("A");
    CHECK_NOTHROW(t.append(cexpr_string<256>("B")));
    REQUIRE(std::strcmp(t.data(), "AB") == 0);
    CHECK_THROWS(t.append(cexpr_string<256>("C")));
  }

  SECTION("Clearing.") {
    auto s = cexpr_string<256>("ABCDEF");
    s.clear();
    REQUIRE(std::strcmp(s.data(), "") == 0);
    REQUIRE(s.size() == 0);
  }
}

TEST_CASE("To string.") {
  char buffer[256] = {};
  char *end = buffer;
  size_t len = to_string(24, buffer, 3, &end);
  REQUIRE(len == 2);
  REQUIRE(std::strcmp("24", buffer) == 0);
  REQUIRE(end == buffer + 2);
  REQUIRE(*end == 0);

  len = to_string(90, buffer, 3, &end);
  REQUIRE(std::strcmp("90", buffer) == 0);
  REQUIRE(len == 2);

  CHECK_THROWS(to_string(777, buffer, 3, &end));
  CHECK_THROWS(to_string(2, buffer, 1, &end));

  len = to_string(0, buffer, 2, &end);
  REQUIRE(len == 1);
  REQUIRE(std::strcmp("0", buffer) == 0);
  REQUIRE(*end == 0);
  REQUIRE(end == buffer + 1);

  len = to_string(1, buffer, 2, &end);
  REQUIRE(len == 1);
  REQUIRE(std::strcmp("1", buffer) == 0);
  REQUIRE(*end == 0);
  REQUIRE(end == buffer + 1);

  len = to_string(999999999999, buffer, 256, &end);
  REQUIRE(len == 12);
  REQUIRE(std::strcmp("999999999999", buffer) == 0);
  REQUIRE(*end == 0);
  REQUIRE(end == buffer + 12);
}
