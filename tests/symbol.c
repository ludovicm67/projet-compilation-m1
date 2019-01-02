#define TEST_NO_MAIN

#include "acutest.h"
#include "symbol.h"
#include "../symbol.h"

void test_symbol_new(void) {
  symbol_t *symbol = NULL;
  TEST_CHECK(symbol == NULL);
  symbol = symbol_new("test", true, 42);
  TEST_CHECK(symbol != NULL);
  TEST_CHECK_(!strcmp(symbol->name, "test"), "Expected name to be 'test' but got %s", symbol->name);
  TEST_CHECK_(symbol->isMutable, "Expected isMutable to be 'true' but got %d", symbol->isMutable);
  TEST_CHECK_(symbol->value, "Expected value to be '42' but got %d", symbol->value);
}
