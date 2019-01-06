#define TEST_NO_MAIN

#include "../quad.h"
#include "acutest.h"

void test_quad_new(void) {
  op_t *quad             = NULL;
  symbol_t *symbol_table = NULL;
  symbol_t *symbol_left  = symbol_add(&symbol_table, SYM_UNKNOWN, "a", true);
  symbol_t *symbol_right = symbol_add(&symbol_table, SYM_UNKNOWN, "b", true);
  symbol_t *result       = symbol_lookup(&symbol_table, "x");
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

void test_quad_list(void) {
  symbol_t *symbol_table = NULL;
  TEST_CHECK(symbol_table == NULL);

  symbol_t *var_y = symbol_add(&symbol_table, SYM_UNKNOWN, "y", false);
  TEST_CHECK(var_y != NULL);
  symbol_t *tmp_1 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  TEST_CHECK(tmp_1 != NULL);
  symbol_t *var_x = symbol_add(&symbol_table, SYM_UNKNOWN, "x", false);
  TEST_CHECK(var_x != NULL);
  symbol_t *var_z = symbol_add(&symbol_table, SYM_UNKNOWN, "z", false);
  TEST_CHECK(var_z != NULL);
  symbol_t *var_y_1 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_1 != NULL);
  TEST_CHECK(var_y == var_y_1);
  symbol_t *var_y_2 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_2 != NULL);
  TEST_CHECK(var_y == var_y_2);

  TEST_CHECK(symbol_table != NULL);
  op_t *quad1     = quad_new(QUAD_OP_ADD, var_y, tmp_1, var_x);
  op_t *quad2     = quad_new(QUAD_OP_MUL, var_z, var_y_1, var_y_2);
  op_t *quad3     = quad_new(QUAD_OP_MUL, var_z, var_y_1, var_y_2);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));

  // check length
  TEST_CHECK(list != NULL);
  TEST_CHECK(list->next != NULL);
  TEST_CHECK(list->next->next == NULL);

  // check order
  TEST_CHECK(list->quad == quad1);
  TEST_CHECK(list->next->quad == quad2);

  // test append
  quad_list_append(&list, quad3);

  // check length
  TEST_CHECK(list != NULL);
  TEST_CHECK(list->next != NULL);
  TEST_CHECK(list->next->next != NULL);
  TEST_CHECK(list->next->next->next == NULL);

  // check order
  TEST_CHECK(list->quad == quad1);
  TEST_CHECK(list->next->quad == quad2);
  TEST_CHECK(list->next->next->quad == quad3);

  // clean
  quad_list_delete(list);
  symbol_delete(symbol_table);
}
