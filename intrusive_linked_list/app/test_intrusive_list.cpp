#define CATCH_CONFIG_MAIN

#include "ra/intrusive_list.hpp"

#include <catch2/catch.hpp>

using namespace ra::intrusive;

class Widget {
public:
  int data;
  list_hook hook;
};

using widget_list = list<Widget, &Widget::hook>;

// TO-DO: Decide whether lists using different hooks should be moveable, etc.

TEST_CASE("Initialization.") {
  SECTION("Default construct.") {
    widget_list wl;
    REQUIRE(wl.size() == 0);
    REQUIRE(wl.begin() == wl.end());
    REQUIRE_THROWS(wl.back());
  }

  SECTION("Move construct.") {
    Widget dummy;
    widget_list wl;
    wl.push_back(dummy);
    widget_list another_list(std::move(wl));
    REQUIRE(another_list.size() == 1);
    REQUIRE(another_list.begin() != wl.begin());
    REQUIRE(another_list.end() != wl.end());
    REQUIRE(&another_list.back() == &dummy);
    // Source list must be empty
    REQUIRE(wl.size() == 0);
  }

  SECTION("Move assignment.") {
    Widget dummy;
    widget_list wl;
    widget_list another_list;
    wl.push_back(dummy);

    another_list = std::move(wl);
    REQUIRE(another_list.size() == 1);
    REQUIRE(another_list.begin() != wl.begin());
    REQUIRE(another_list.end() != wl.end());
    REQUIRE(&another_list.back() == &dummy);
    // Source list must be empty
    REQUIRE(wl.size() == 0);
  }
}

TEST_CASE("Non-mutating functions.") {
  SECTION("Size.") {
    Widget ws[10];
    widget_list wl;
    widget_list another_list;
    REQUIRE(wl.size() == 0);
    for (int i = 0; i < 10; ++i) {
      wl.push_back(ws[i]);
      another_list.push_back(ws[i]);
    }
    REQUIRE(wl.size() == 10);
    wl.erase(wl.begin());
    REQUIRE(wl.size() == 9);
    wl.clear();
    REQUIRE(wl.size() == 0);
    wl.swap(another_list);
    REQUIRE(wl.size() == 10);
  }

  SECTION("Back.") {
    Widget w;
    widget_list wl;
    wl.push_back(w);
    const widget_list another_list = std::move(wl);
    REQUIRE(&another_list.back() == &w);
    REQUIRE_THROWS(widget_list().back());
  }

  SECTION("Begin / End.") {
    Widget w;
    widget_list wl;
    wl.push_back(w);
    REQUIRE(&(*wl.begin()) == &w);
    REQUIRE(++wl.begin() == wl.end());
    REQUIRE(--wl.end() == wl.begin());
  }
}

TEST_CASE("Mutating functions.") {
  SECTION("Insert.") {
    Widget ws[5];
    widget_list wl;
    for (int i = 0; i < 5; ++i) {
      wl.insert(wl.end(), ws[i]);
    }
    for (auto [it, i] = std::pair(wl.begin(), 0); it != wl.end(); ++it, ++i) {
      REQUIRE(&(*it) == ws + i);
    }
  }

  SECTION("Erase.") {
    Widget ws[5];
    widget_list wl;
    for (int i = 0; i < 5; ++i) {
      wl.insert(wl.end(), ws[i]);
    }
    auto it = wl.begin();
    ++it;
    ++it;
    it = wl.erase(it);
    REQUIRE(&(*it) == ws + 3);
    REQUIRE(&(*(--it)) == ws + 1);
    // Restore
    ++it;
    REQUIRE(&(*(++it)) == ws + 4);
  }
  

  SECTION("Push back and pop back.") {
    // Push back extensively tested in other sections
    Widget w;
    widget_list wl;
    wl.push_back(w);
    wl.pop_back();
    REQUIRE(wl.size() == 0);
    REQUIRE(wl.begin() == wl.end());
    REQUIRE_THROWS(widget_list().pop_back());
  }
}
