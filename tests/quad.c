#define TEST_NO_MAIN

#include "acutest.h"
#include "../quad.h"

void test_quad_test(void) {
  int a, b;

  a = 1;
  b = 2;
  TEST_CHECK_(a + b == 3, "Expected %d, got %d", 3, a + b);
}

