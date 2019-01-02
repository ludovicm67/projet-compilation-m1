#define TEST_NO_MAIN

#include "../quad.h"
#include "acutest.h"

void test_quad_new(void) {
  op_t *quad = NULL;
  symbol_t *symbol_table = NULL;
  symbol_t *symbol_left = symbol_add(&symbol_table, "a", true, 1);
  symbol_t *symbol_right = symbol_add(&symbol_table, "b", true, 2);
  symbol_t *result = symbol_lookup(&symbol_table, "x");
  TEST_CHECK(quad == NULL);
  quad = quad_new(QUAD_OP_ADD, result, symbol_left, symbol_right);
  TEST_CHECK(quad != NULL);
  TEST_CHECK(quad->op == QUAD_OP_ADD);
  TEST_CHECK(quad->q1 == result);
  TEST_CHECK(quad->q2 == symbol_left);
  TEST_CHECK(quad->q3 == symbol_right);
  quad_delete(quad);
  symbol_delete(symbol_table);
}
