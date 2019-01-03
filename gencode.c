#include "gencode.h"
#include <stdio.h>

void gencode_init(symbol_t *symbol_table, uint32_t precision) {
  uint32_t n = 0;
  char *indent = "  ";

  if (symbol_table) {
    printf("\n%s// declaration of all MPC variables that we be using\n",
           indent);
  }

  while (symbol_table) {
    printf("%smpc_t T%d; mpc_init2(T%d, %d);\n", indent, n, n, precision);
    symbol_table->number = n++;
    symbol_table = symbol_table->next;
  }
}

// @TODO: add precision parameter
void gencode_assign(symbol_t *symbol_table) {
  char *indent = "  ";

  if (symbol_table) {
    printf("\n%s// assign values to some variables\n", indent);
  }

  while (symbol_table) {
    if (symbol_table->external && symbol_table->name) {
      printf("%smpc_set_si(T%d, %s, MPC_RNDZZ); // %s\n", indent,
             symbol_table->number, symbol_table->name, symbol_table->name);
    } else if (symbol_table->hasValue) {
      printf("%smpc_set_si(T%d, %f, MPC_RNDZZ);\n", indent,
             symbol_table->number, symbol_table->value);
    }
    symbol_table = symbol_table->next;
  }
}

void gencode_clear(symbol_t *symbol_table) {
  char *indent = "  ";

  if (symbol_table) {
    printf("\n%s// free memory of all MPC variables that we used\n", indent);
  }

  while (symbol_table) {
    printf("%smpc_clear(T%d);\n", indent, symbol_table->number);
    symbol_table = symbol_table->next;
  }
}
