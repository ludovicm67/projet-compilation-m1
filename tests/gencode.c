#define TEST_NO_MAIN

#include "../gencode.h"
#include "../quad.h"
#include "acutest.h"

void test_gencode_init(void) {
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, true, 1);
  symbol_add(&symbol_table, "a", true, true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");
  printf("\nOUTPUT:\n");
  gencode_init(symbol_table, 128);
  symbol_delete(symbol_table);
}

void test_gencode_clear(void) {
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, true, 1);
  symbol_add(&symbol_table, "a", true, true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");
  gencode_init(symbol_table, 128);
  printf("\nOUTPUT:\n");
  gencode_clear(symbol_table, "MPC_RNDZZ");
  symbol_delete(symbol_table);
}

void test_gencode_example(void) {
  symbol_t *symbol_table = NULL;
  TEST_CHECK(symbol_table == NULL);

  symbol_t *var_y = symbol_add(&symbol_table, "y", false, false, 0);
  TEST_CHECK(var_y != NULL);
  symbol_t *tmp_1 = symbol_add(&symbol_table, NULL, false, true, 1);
  TEST_CHECK(tmp_1 != NULL);
  symbol_t *var_x = symbol_lookup(&symbol_table, "x");
  TEST_CHECK(var_x != NULL);
  symbol_t *var_z = symbol_lookup(&symbol_table, "z");
  TEST_CHECK(var_z != NULL);
  symbol_t *var_y_1 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_1 != NULL);
  TEST_CHECK(var_y == var_y_1);
  symbol_t *var_y_2 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_2 != NULL);
  TEST_CHECK(var_y == var_y_2);

  TEST_CHECK(symbol_table != NULL);
  op_t *quad1 = quad_new(QUAD_OP_ADD, var_y, tmp_1, var_x);
  op_t *quad2 = quad_new(QUAD_OP_MUL, var_z, var_y_1, var_y_2);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));

  // check length
  TEST_CHECK(list != NULL);
  TEST_CHECK(list->next != NULL);
  TEST_CHECK(list->next->next == NULL);

  // check order
  TEST_CHECK(list->quad == quad1);
  TEST_CHECK(list->next->quad == quad2);

  printf("\nOUTPUT:\n");
  gencode_init(symbol_table, 128);
  gencode_assign(symbol_table, "MPC_RNDZZ");
  gencode_operations(list, "MPC_RNDZZ");
  gencode_clear(symbol_table, "MPC_RNDZZ");

  // clean
  quad_list_delete(list);
  quad_delete(quad1);
  quad_delete(quad2);
  symbol_delete(symbol_table);
}
