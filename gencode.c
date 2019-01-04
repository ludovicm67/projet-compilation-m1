#include "gencode.h"

#define ARG_1                                                                  \
  indent, q->q1->number, q->q1->number, args->rounding, q->q1->number
#define ARG_2                                                                  \
  indent, q->q1->number, q->q2->number, args->rounding, q->q1->number,         \
      q->q2->number
#define ARG_3                                                                  \
  indent, q->q1->number, q->q2->number, q->q3->number, args->rounding,         \
      q->q1->number, q->q2->number, q->q3->number

void gencode_init(gencode_args_t *args, symbol_t *symbol_table) {
  uint32_t n = 0;
  char *indent = "  ";

  if (symbol_table) {
    fprintf(args->file,
            "\n%s// declaration of all MPC variables that we will use\n",
            indent);
  }

  while (symbol_table) {
    fprintf(args->file, "%smpc_t T%d; mpc_init2(T%d, %d);", indent, n, n,
            args->precision);
    if (symbol_table->name)
      fprintf(args->file, " // %s\n", symbol_table->name);
    else
      fprintf(args->file, "\n");
    symbol_table->number = n++;
    symbol_table = symbol_table->next;
  }
}

void gencode_assign(gencode_args_t *args, symbol_t *symbol_table) {
  char *indent = "  ";

  if (symbol_table) {
    fprintf(args->file, "\n%s// assign values to some variables\n", indent);
  }

  while (symbol_table) {
    if (symbol_table->external && symbol_table->name &&
        symbol_table->readBeforeModified) {
      fprintf(args->file, "%smpc_set_d(T%d, %s, %s); // %s\n", indent,
              symbol_table->number, symbol_table->name, args->rounding,
              symbol_table->name);
    } else if (symbol_table->hasValue) {
      fprintf(args->file, "%smpc_set_d(T%d, %f, %s);\n", indent,
              symbol_table->number, symbol_table->value, args->rounding);
    }
    symbol_table = symbol_table->next;
  }
}

void gencode_operations(gencode_args_t *args, op_list_t *list) {
  char *indent = "  ";
  op_t *q;

  if (list) {
    fprintf(args->file, "\n%s// operations\n", indent);
  }

  while (list) {
    q = list->quad;
    switch (q->op) {
    case QUAD_OP_ADD:
      fprintf(args->file, "%smpc_add(T%d, T%d, T%d, %s); // T%d = T%d + T%d\n",
              ARG_3);
      break;

    case QUAD_OP_SUB:
      fprintf(args->file, "%smpc_sub(T%d, T%d, T%d, %s); // T%d = T%d - T%d\n",
              ARG_3);
      break;

    case QUAD_OP_MUL:
      fprintf(args->file, "%smpc_mul(T%d, T%d, T%d, %s); // T%d = T%d * T%d\n",
              ARG_3);
      break;

    case QUAD_OP_DIV:
      fprintf(args->file, "%smpc_div(T%d, T%d, T%d, %s); // T%d = T%d / T%d\n",
              ARG_3);
      break;

    case QUAD_OP_ASSIGN:
      fprintf(args->file, "%smpc_set(T%d, T%d, %s); // T%d = T%d\n", ARG_2);
      break;

    case QUAD_OP_SQRT:
      fprintf(args->file, "%smpc_sqr(T%d, T%d, %s); // T%d = sqrt(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_NEG:
      fprintf(args->file, "%smpc_neg(T%d, T%d, %s); // T%d = -T%d\n", ARG_2);
      break;

    case QUAD_OP_ABS:
      fprintf(args->file, "%smpc_abs(T%d, T%d, %s); // T%d = abs(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_EXP:
      fprintf(args->file, "%smpc_exp(T%d, T%d, %s); // T%d = exp(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_LOG:
      fprintf(args->file, "%smpc_log(T%d, T%d, %s); // T%d = log(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_POW:
      fprintf(args->file, "%smpc_pow(T%d, T%d, %s); // T%d = pow(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_SIN:
      fprintf(args->file, "%smpc_sin(T%d, T%d, %s); // T%d = sin(T%d)\n",
              ARG_2);
      break;

    case QUAD_OP_INCR:
      fprintf(args->file, "%smpc_add_ui(T%d, T%d, 1, %s); // T%d++\n", ARG_1);
      break;

    case QUAD_OP_DECR:
      fprintf(args->file, "%smpc_sub_ui(T%d, T%d, 1, %s); // T%d--\n", ARG_1);
      break;

    case QUAD_NOOP:
      fprintf(args->file, "%s// Unsupported OP\n", indent);
      break;
    }
    list = list->next;
  }
}

void gencode_clear(gencode_args_t *args, symbol_t *symbol_table) {
  symbol_t *s;
  char *indent = "  ";

  for (s = symbol_table; s; s = s->next) {
    if (s->modified && s->name && s->external) {
      fprintf(args->file, "%s%s = mpc_get_dc(T%d, %s);\n", indent, s->name,
              s->number, args->rounding);
    }
  }

  if (symbol_table) {
    fprintf(args->file,
            "\n%s// free memory of all MPC variables that we used\n", indent);
  }

  for (s = symbol_table; s; s = s->next) {
    fprintf(args->file, "%smpc_clear(T%d);\n", indent, s->number);
  }
}
