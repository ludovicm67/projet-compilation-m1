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
