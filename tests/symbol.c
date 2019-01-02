#define TEST_NO_MAIN

#include "../symbol.h"
#include "acutest.h"

void test_symbol_memory(void) {
  symbol_t *symbol = NULL;
  TEST_CHECK(symbol == NULL);
  symbol = symbol_new("test", true, 42);
  TEST_CHECK(symbol != NULL);
  TEST_CHECK_(!strcmp(symbol->name, "test"),
              "Expected name to be 'test' but got %s", symbol->name);
  TEST_CHECK_(symbol->external, "Expected external to be 'true' but got %d",
              symbol->external);
  TEST_CHECK_(!symbol->modified, "Expected modified to be 'false' but got %d",
              symbol->modified);
  TEST_CHECK_(symbol->value, "Expected value to be '42' but got %d",
              symbol->value);
  symbol_delete(symbol);
}

void test_symbol_same_name(void) {
  symbol_t *symbol_table = NULL;
  symbol_t *symbol_left = symbol_add(&symbol_table, "a", true, 1);
  symbol_t *symbol_right = symbol_add(&symbol_table, "a", true, 2);
  symbol_t *result = symbol_lookup(&symbol_table, "x");
  symbol_t *same_as_result = symbol_lookup(&symbol_table, "x");
  TEST_CHECK(result == same_as_result);
  TEST_CHECK(symbol_table == result);
  TEST_CHECK(symbol_table->next == symbol_right);
  TEST_CHECK(symbol_table->next->value == 2);
  TEST_CHECK(symbol_table->next->next == symbol_left);
  TEST_CHECK(symbol_table->next->next->value == 1);
  TEST_CHECK(symbol_table->next->next->next == NULL);
  symbol_delete(symbol_table);
}
