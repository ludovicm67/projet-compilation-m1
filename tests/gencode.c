#define TEST_NO_MAIN

#include "../gencode.h"
#include "acutest.h"

void test_gencode_init(void) {
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, 1);
  symbol_add(&symbol_table, "a", true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");
  printf("\nOUTPUT:\n");
  gencode_init(symbol_table, 128);
  symbol_delete(symbol_table);
}

void test_gencode_clear(void) {
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, 1);
  symbol_add(&symbol_table, "a", true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");
  printf("\nOUTPUT:\n");
  gencode_clear(symbol_table);
  symbol_delete(symbol_table);
}
