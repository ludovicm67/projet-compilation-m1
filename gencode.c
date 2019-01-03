#include "gencode.h"
#include <stdio.h>

void gencode_init(symbol_t *symbol_table, uint32_t precision) {
  uint32_t n = 0;
  while (symbol_table) {
    printf("mpc_t T%d; mpc_init2(T%d, %d);\n", n, n, precision);
    symbol_table = symbol_table->next;
    n++;
  }
}

void gencode_clear(symbol_t *symbol_table) {
  uint32_t n = 0;
  while (symbol_table) {
    printf("mpc_clear(T%d);\n", n++);
    symbol_table = symbol_table->next;
  }
}
