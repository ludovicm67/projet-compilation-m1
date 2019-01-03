#include "gencode.h"
#include <stdio.h>

void gencode_init(symbol_t *symbol_table, uint32_t precision) {
  uint32_t n = 0;
  char *indent = "  ";

  if (symbol_table) {
    printf("\n%s// declaration of all MPC variables that we will use\n",
           indent);
  }

  while (symbol_table) {
    printf("%smpc_t T%d; mpc_init2(T%d, %d);", indent, n, n, precision);
    if (symbol_table->name)
      printf(" // %s\n", symbol_table->name);
    else
      printf("\n");
    symbol_table->number = n++;
    symbol_table = symbol_table->next;
  }
}

void gencode_assign(symbol_t *symbol_table, char *rounding) {
  char *indent = "  ";

  if (symbol_table) {
    printf("\n%s// assign values to some variables\n", indent);
  }

  while (symbol_table) {
    if (symbol_table->external && symbol_table->name) {
      printf("%smpc_set_si(T%d, %s, %s); // %s\n", indent, symbol_table->number,
             symbol_table->name, rounding, symbol_table->name);
    } else if (symbol_table->hasValue) {
      printf("%smpc_set_si(T%d, %f, %s);\n", indent, symbol_table->number,
             symbol_table->value, rounding);
    }
    symbol_table = symbol_table->next;
  }
}

void gencode_operations(op_list_t *list, char *rounding) {
  char *indent = "  ";
  op_t *q;

  if (list) {
    printf("\n%s// operations\n", indent);
  }

  while (list) {
    q = list->quad;
    switch (q->op) {
    case QUAD_OP_ADD:
      printf("%smpc_add(T%d, T%d, T%d, %s); // T%d = T%d + T%d \n", indent,
             q->q1->number, q->q2->number, q->q3->number, rounding,
             q->q1->number, q->q2->number, q->q3->number);
      break;
    case QUAD_OP_MUL:
      printf("%smpc_mul(T%d, T%d, T%d, %s); // T%d = T%d * T%d \n", indent,
             q->q1->number, q->q2->number, q->q3->number, rounding,
             q->q1->number, q->q2->number, q->q3->number);
      break;
    }
    list = list->next;
  }
}

void gencode_clear(symbol_t *symbol_table, char *rounding) {
  symbol_t *s;
  char *indent = "  ";

  for (s = symbol_table; s; s = s->next) {
    if (s->modified && s->name) {
      printf("%s%s = mpc_get_ldc(T%d, %s);\n", indent, s->name, s->number,
             rounding);
    }
  }

  if (symbol_table) {
    printf("\n%s// free memory of all MPC variables that we used\n", indent);
  }

  for (s = symbol_table; s; s = s->next) {
    printf("%smpc_clear(T%d);\n", indent, s->number);
  }
}
