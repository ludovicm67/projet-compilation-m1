#include <stdlib.h>

#include "gencode.h"

#define ARG_1                                                                  \
  indent, __gencode_lib_name(args->lib), q->q1->number, q->q1->number,         \
      args->rounding, q->q1->number
#define ARG_2                                                                  \
  indent, __gencode_lib_name(args->lib), q->q1->number, q->q2->number,         \
      args->rounding, q->q1->number, q->q2->number
#define ARG_3                                                                  \
  indent, __gencode_lib_name(args->lib), q->q1->number, q->q2->number,         \
      q->q3->number, args->rounding, q->q1->number, q->q2->number,             \
      q->q3->number

char *__gencode_lib_name(gencode_lib_t lib) {
  if (lib == LIB_MPFR)
    return "mpfr";
  return "mpc";
}

void gencode_init(gencode_args_t *args, symbol_t *symbol_table) {
  uint32_t n = 0;
  char *indent = "  ";
  char *lib = __gencode_lib_name(args->lib);

  if (symbol_table) {
    fprintf(args->file,
            "\n%s// declaration of all variables that we will use\n", indent);
  }

  while (symbol_table) {
    fprintf(args->file, "%s%s_t T%d; %s_init2(T%d, %d);", indent, lib, n, lib,
            n, args->precision);
    if (symbol_table->name)
      fprintf(args->file, " // %s\n", symbol_table->name);
    else
      fprintf(args->file, "\n");
    symbol_table->number = n++;
    symbol_table = symbol_table->next;
  }
}

void gencode_assign(gencode_args_t *args, symbol_t *symbol) {
  char *indent = "  ";
  char *lib = __gencode_lib_name(args->lib);

  if (symbol) {
    fprintf(args->file, "\n%s// assign values to some variables\n", indent);
  }

  while (symbol) {
    if (symbol->name && symbol->readBeforeModified && !symbol->declared) {
      fprintf(args->file, "%s%s_set_d(T%d, %s, %s); // %s\n", indent, lib,
              symbol->number, symbol->name, args->rounding, symbol->name);
    } else if (symbol->hasValue) {
      if (symbol->type == SYM_DECIMAL) {
        fprintf(args->file, "%s%s_set_d(T%d, %f, %s);\n", indent, lib,
                symbol->number, symbol->value.decimal, args->rounding);
      } else if (symbol->type == SYM_INTEGER) {
        fprintf(args->file, "%s%s_set_d(T%d, %d, %s);\n", indent, lib,
                symbol->number, symbol->value.integer, args->rounding);
      } else {
        fprintf(stderr, "Unsupported symbol type %d\n", symbol->type);
        fflush(stderr);
        abort();
      }
    }
    symbol = symbol->next;
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
        fprintf(args->file, "%s%s_add(T%d, T%d, T%d, %s); // T%d = T%d + T%d\n",
                ARG_3);
        break;

      case QUAD_OP_SUB:
        fprintf(args->file, "%s%s_sub(T%d, T%d, T%d, %s); // T%d = T%d - T%d\n",
                ARG_3);
        break;

      case QUAD_OP_MUL:
        fprintf(args->file, "%s%s_mul(T%d, T%d, T%d, %s); // T%d = T%d * T%d\n",
                ARG_3);
        break;

      case QUAD_OP_DIV:
        fprintf(args->file, "%s%s_div(T%d, T%d, T%d, %s); // T%d = T%d / T%d\n",
                ARG_3);
        break;

      case QUAD_OP_ASSIGN:
        fprintf(args->file, "%s%s_set(T%d, T%d, %s); // T%d = T%d\n", ARG_2);
        break;

      case QUAD_OP_SQRT:
        fprintf(args->file, "%s%s_sqr(T%d, T%d, %s); // T%d = sqrt(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_NEG:
        fprintf(args->file, "%s%s_neg(T%d, T%d, %s); // T%d = -T%d\n", ARG_2);
        break;

      case QUAD_OP_ABS:
        fprintf(args->file, "%s%s_abs(T%d, T%d, %s); // T%d = abs(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_EXP:
        fprintf(args->file, "%s%s_exp(T%d, T%d, %s); // T%d = exp(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_LOG:
        fprintf(args->file, "%s%s_log(T%d, T%d, %s); // T%d = log(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_POW:
        fprintf(args->file, "%s%s_pow(T%d, T%d, %s); // T%d = pow(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_SIN:
        fprintf(args->file, "%s%s_sin(T%d, T%d, %s); // T%d = sin(T%d)\n",
                ARG_2);
        break;

      case QUAD_OP_INCR:
        fprintf(args->file, "%s%s_add_ui(T%d, T%d, 1, %s); // T%d++\n", ARG_1);
        break;

      case QUAD_OP_DECR:
        fprintf(args->file, "%s%s_sub_ui(T%d, T%d, 1, %s); // T%d--\n", ARG_1);
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
  char *lib = __gencode_lib_name(args->lib);

  for (s = symbol_table; s; s = s->next) {
    if (s->modified && s->name) {
      if (s->declared) {
        char *type;
        if (s->type == SYM_DECIMAL)
          type = "double";
        else if (s->type == SYM_INTEGER)
          type = "int";
        else
          abort(); // TODO(sandhose): error handling

        fprintf(args->file, "%s%s %s = %s_get_dc(T%d, %s);\n", indent, type,
                s->name, lib, s->number, args->rounding);
      } else {
        fprintf(args->file, "%s%s = %s_get_dc(T%d, %s);\n", indent, s->name,
                lib, s->number, args->rounding);
      }
    }
  }

  if (symbol_table) {
    fprintf(args->file, "\n%s// free memory of all variables that we used\n",
            indent);
  }

  for (s = symbol_table; s; s = s->next) {
    fprintf(args->file, "%s%s_clear(T%d);\n", indent, lib, s->number);
  }
}
