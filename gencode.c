#include "gencode.h"

void gencode_init(FILE *file, symbol_t *symbol_table, uint32_t precision) {
  uint32_t n = 0;
  char *indent = "  ";

  if (symbol_table) {
    fprintf(file, "\n%s// declaration of all MPC variables that we will use\n",
            indent);
  }

  while (symbol_table) {
    fprintf(file, "%smpc_t T%d; mpc_init2(T%d, %d);", indent, n, n, precision);
    if (symbol_table->name)
      fprintf(file, " // %s\n", symbol_table->name);
    else
      fprintf(file, "\n");
    symbol_table->number = n++;
    symbol_table = symbol_table->next;
  }
}

void gencode_assign(FILE *file, symbol_t *symbol_table, char *rounding) {
  char *indent = "  ";

  if (symbol_table) {
    fprintf(file, "\n%s// assign values to some variables\n", indent);
  }

  while (symbol_table) {
    if (symbol_table->external && symbol_table->name) {
      fprintf(file, "%smpc_set_d(T%d, %s, %s); // %s\n", indent,
              symbol_table->number, symbol_table->name, rounding,
              symbol_table->name);
    } else if (symbol_table->hasValue) {
      fprintf(file, "%smpc_set_d(T%d, %f, %s);\n", indent,
              symbol_table->number, symbol_table->value, rounding);
    }
    symbol_table = symbol_table->next;
  }
}

void gencode_operations(FILE *file, op_list_t *list, char *rounding) {
  char *indent = "  ";
  op_t *q;

  if (list) {
    fprintf(file, "\n%s// operations\n", indent);
  }

  while (list) {
    q = list->quad;
    switch (q->op) {
    case QUAD_OP_ADD:
      fprintf(file, "%smpc_add(T%d, T%d, T%d, %s); // T%d = T%d + T%d\n",
              indent, q->q1->number, q->q2->number, q->q3->number, rounding,
              q->q1->number, q->q2->number, q->q3->number);
      break;
    case QUAD_OP_MUL:
      fprintf(file, "%smpc_mul(T%d, T%d, T%d, %s); // T%d = T%d * T%d\n",
              indent, q->q1->number, q->q2->number, q->q3->number, rounding,
              q->q1->number, q->q2->number, q->q3->number);
      break;
    }
    list = list->next;
  }
}

void gencode_clear(FILE *file, symbol_t *symbol_table, char *rounding) {
  symbol_t *s;
  char *indent = "  ";

  for (s = symbol_table; s; s = s->next) {
    if (s->modified && s->name) {
      fprintf(file, "%s%s = mpc_get_ldc(T%d, %s);\n", indent, s->name,
              s->number, rounding);
    }
  }

  if (symbol_table) {
    fprintf(file, "\n%s// free memory of all MPC variables that we used\n",
            indent);
  }

  for (s = symbol_table; s; s = s->next) {
    fprintf(file, "%smpc_clear(T%d);\n", indent, s->number);
  }
}
