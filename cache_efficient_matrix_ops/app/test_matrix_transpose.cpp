#define CATCH_CONFIG_MAIN

#include "ra/matrix_transpose.hpp"

#include <catch2/catch.hpp>

using namespace ra::cache;

TEST_CASE("Naive matrix transpose.") {
  SECTION("In-place, square matrix.") {
    int a[100];
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        a[i * 10 + j] = i * 10 + j;
      }
    }
    naive_matrix_transpose(a, 10, 10, a);
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        REQUIRE(a[i * 10 + j] == j * 10 + i);
      }
    }
  }
  SECTION("Transpose of rectangular matrix.") {
    int a[50];
    int b[50];
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 5; ++j) {
        a[i * 5 + j] = i * 5 + j;
      }
    }
    naive_matrix_transpose(a, 10, 5, b);
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 5; ++j) {
        REQUIRE(b[j * 10 + i] == a[i * 5 + j]);
      }
    }
  }
}

TEST_CASE("Cache oblivious matrix transpose.") {
  SECTION("In-place square transpose.") {
    int a[100];
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        a[i * 10 + j] = i * 10 + j;
      }
    }
    matrix_transpose(a, 10, 10, a);
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        REQUIRE(a[i * 10 + j] == j * 10 + i);
      }
    }
  }
  
  SECTION("Transpose of rectangular matrix.") {
    int a[50];
    int b[50];
    int c[50];
    int d[50];
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 5; ++j) {
        a[i * 5 + j] = i * 5 + j;
        c[i * 5 + j] = i * 5 + j;
      }
    }
    naive_matrix_transpose(a, 10, 5, b);
    matrix_transpose(c, 10, 5, d);
    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 5; ++j) {
        REQUIRE(b[j * 10 + i] == d[j * 10 + i]); 
      }
    }
  }
  SECTION("Dynamic storage duration.") {
    int *a = new int[10000];
    int *b = new int[10000];
    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < 100; ++j) {
        a[i * 100 + j] = j * i + i + j;
        b[i * 100 + j] = j * i + i + j;
      }
    }
    naive_matrix_transpose(a, 100, 100, a);
    matrix_transpose(b, 100, 100, b);
    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < 100; ++j) {
        REQUIRE(b[i * 100 + j] == a[i * 100 + j]);
      }
    }
    delete [] a;
    delete [] b;
  }
}
