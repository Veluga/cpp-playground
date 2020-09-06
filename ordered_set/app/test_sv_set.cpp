#define CATCH_CONFIG_MAIN

#include "ra/sv_set.hpp"

#include <catch2/catch.hpp>

using namespace ra::container;

class test_helper {
public:
  inline static std::size_t count = 0;
  ~test_helper() { ++count; }
};

TEST_CASE("Construction.") {
  SECTION("Default construct.") {
    const sv_set<double> s;
    REQUIRE(s.begin() == nullptr);
    REQUIRE(s.end() == nullptr);
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 0);
  }

  SECTION("Construct from range.") {
    int data[] = {1, 2, 3};
    sv_set<int>::ordered_and_unique_range dummy;

    sv_set<int> s = {dummy, data, 3};
    REQUIRE(s.size() == 3);
    REQUIRE(s.capacity() == 3);
    REQUIRE(*(s.begin()) == 1);
    REQUIRE(s.end() == s.begin() + s.size());

    sv_set<int> t{dummy, data, 0};
    REQUIRE(t.size() == 0);
    REQUIRE(t.capacity() == 0);
    REQUIRE(t.end() == t.begin());
  }

  SECTION("Move construct.") {
    int data[] = {1, 2, 3};
    sv_set<int>::ordered_and_unique_range dummy;

    sv_set<int> s = {dummy, data, 3};
    sv_set<int> t(std::move(s));
    REQUIRE(t.size() == 3);
    REQUIRE(t.capacity() == 3);
    REQUIRE(t.end() == t.begin() + 3);
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 0);
    REQUIRE(s.begin() == nullptr);
    REQUIRE(s.end() == nullptr);
  }

  SECTION("Move assignment.") {
    test_helper arr[42];
    sv_set<test_helper>::ordered_and_unique_range ran;

    sv_set<test_helper> s{ran, arr, 42};
    s = sv_set<test_helper>();
    REQUIRE(test_helper::count == 42);
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 0);
    REQUIRE(s.begin() == nullptr);
    REQUIRE(s.end() == nullptr);
  }

  SECTION("Copy constructor.") {
    int arr[] = {51, 50, 49}; 
    sv_set<int, std::greater<int>>::ordered_and_unique_range ran;

    sv_set<int, std::greater<int>> s{ran, arr, 3};
    sv_set<int, std::greater<int>> t(s);
    REQUIRE(t.size() == s.size());
    REQUIRE(t.capacity() == s.capacity());
    REQUIRE(t.begin() != s.begin());
    REQUIRE(*(t.begin()) == *(s.begin()));
  }

  SECTION("Copy assignment.") {
    int arr[] = {51, 50, 49}; 
    sv_set<int, std::greater<int>>::ordered_and_unique_range ran;

    sv_set<int, std::greater<int>> s{ran, arr, 3};
    sv_set<int, std::greater<int>> t = s;
    REQUIRE(t.size() == s.size());
    REQUIRE(t.capacity() == s.capacity());
    REQUIRE(t.begin() != s.begin());
    REQUIRE(*(t.begin()) == *(s.begin()));
  }
}

TEST_CASE("Dynamic use of values.") {
  SECTION("Find and erase values.") {
    int test_case[] = {1, 2, 3, 4, 5, 6};
    auto s = sv_set<int>(sv_set<int>::ordered_and_unique_range(), test_case, 6);
    REQUIRE(s.find(3) == s.begin() + 2);
    REQUIRE(s.find(7) == s.end());

    int test_case_2[] = {6, 5, 4, 3, 2, 1};
    auto t = sv_set<int, std::greater<int>>(
        sv_set<int, std::greater<int>>::ordered_and_unique_range(), test_case_2,
        6);
    REQUIRE(t.find(3) == t.begin() + 3);
    REQUIRE(t.find(7) == t.end());

    sv_set<int> u;
    u.insert(1);
    u.insert(2);
    u.insert(9);
    u.insert(12);
    REQUIRE(u.find(9) == u.begin() + 2);
    REQUIRE(u.end() == u.begin() + u.size());
    u.erase(u.begin());
    REQUIRE(u.size() == 3);
    u.erase(u.begin());
    REQUIRE(u.find(9) == u.begin());
    REQUIRE(u.size() == 2);
    REQUIRE(u.erase(u.begin()) == u.begin());
    REQUIRE(u.size() == 1);
    u.erase(u.begin());
    REQUIRE(u.size() == 0);
    REQUIRE(u.end() == u.begin() + u.size());

    REQUIRE(u.erase(u.end()) == u.end()); 
    
    u.insert(13);
    u.insert(19);
    u.insert(15);
    REQUIRE(u.find(13) != u.end());
    REQUIRE(u.find(19) != u.end());
    REQUIRE(u.find(15) != u.end());
    REQUIRE(u.size() == 3);
    REQUIRE(u.end() == u.begin() + u.size());
    REQUIRE(u.capacity() == 4);

    REQUIRE(!u.insert(13).second);
  }

  SECTION("Growing and shrinking container.") {
    sv_set<char> s;
    s.reserve(1000000);
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 1000000);

    s.reserve(3);
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 1000000);

    s.shrink_to_fit();
    REQUIRE(s.size() == 0);
    REQUIRE(s.capacity() == 0);
  }
}
