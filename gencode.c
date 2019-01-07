#include <stdlib.h>

#include "gencode.h"
#include "util.h"

#define PREFIX "c2mp_"
#define LABEL PREFIX "label"
#define TEMP PREFIX "temp"
#define BOOL PREFIX "bool"

#define ARG_1                                                                  \
  indent, gencode_lib_name(args->lib), q->q1->number, q->q1->number,           \
      args->rounding, q->q1->number
#define ARG_2                                                                  \
  indent, gencode_lib_name(args->lib), q->q1->number, q->q2->number,           \
      args->rounding, q->q1->number, q->q2->number
#define ARG_3                                                                  \
  indent, gencode_lib_name(args->lib), q->q1->number, q->q2->number,           \
      q->q3->number, args->rounding, q->q1->number, q->q2->number,             \
      q->q3->number

static inline char *gencode_lib_name(gencode_lib_t lib) {
  if (lib == LIB_MPFR)
    return "mpfr";
  return "mpc";
}

void gencode_init(gencode_args_t *args, symbol_t *symbol) {
  static uint32_t n = 0;
  char *indent      = "  ";
  char *lib         = gencode_lib_name(args->lib);

  if (symbol) {
    INFO("Generating the init section");

    fprintf(args->file,
            "\n%s// declaration of all variables that we will use\n", indent);
  }

  for (; symbol; symbol = symbol->next) {
    if (symbol->alias)
      continue;

    if (symbol->type == SYM_UNKNOWN) {
      WARNF("Could not infer type for symbol %p, assuming decimal",
            (void *)symbol);
    }
    switch (symbol->type) {
      case SYM_UNKNOWN:
      case SYM_DECIMAL:
        fprintf(args->file, "%s%s_t " TEMP "%d; %s_init2(" TEMP "%d, %d);",
                indent, lib, n, lib, n, args->precision);
        break;

      case SYM_INTEGER:
        FATAL("Integers are not supported");
        break;

      case SYM_BOOLEAN:
        fprintf(args->file, "%sbool " BOOL "%d;", indent, n);
        break;

      case SYM_LABEL:
        fprintf(args->file, "%s// " LABEL "%d", indent, n);
        break;
    }

    if (symbol->name)
      fprintf(args->file, " // %s\n", symbol->name);
    else
      fprintf(args->file, "\n");
    symbol->number = n++;
  }
}

void gencode_assign(gencode_args_t *args, symbol_t *symbol) {
  char *indent = "  ";
  char *lib    = gencode_lib_name(args->lib);

  if (symbol) {
    INFO("Generating the assign section");
    fprintf(args->file, "\n%s// assign values to some variables\n", indent);
  }

  for (; symbol; symbol = symbol->next) {
    // TODO(sandhose): support boolean
    if (symbol->type == SYM_LABEL || symbol->type == SYM_BOOLEAN)
      continue;

    symbol_t *target = symbol;
    while (target->alias)
      target = target->alias;
    if (target->assigned) {
      DEBUGF("Skipping symbol " TEMP "%d, already assigned.", target->number);
      continue;
    }

    if (symbol->name && symbol->readBeforeModified && !symbol->declared) {
      target->assigned = true;
      fprintf(args->file, "%s%s_set_d(" TEMP "%d, %s, %s); // %s\n", indent,
              lib, target->number, symbol->name, args->rounding, symbol->name);
    } else if (symbol->hasValue) {
      target->assigned = true;
      if (symbol->type == SYM_DECIMAL) {
        fprintf(args->file, "%s%s_set_d(" TEMP "%d, %f, %s);\n", indent, lib,
                target->number, symbol->value.decimal, args->rounding);
      } else if (symbol->type == SYM_INTEGER) {
        fprintf(args->file, "%s%s_set_d(" TEMP "%d, %d, %s);\n", indent, lib,
                target->number, symbol->value.integer, args->rounding);
      } else {
        fprintf(stderr, "Unsupported symbol type %d\n", symbol->type);
        fflush(stderr);
        abort();
      }
    }
  }
}

void gencode_operations(gencode_args_t *args, op_list_t *list) {
  char *indent = "  ";
  op_t *q;

  if (list) {
    INFO("Generating the operations section");
    fprintf(args->file, "\n%s// operations\n", indent);
  }

  for (; list; list = list->next) {
    q = list->quad;
    if (!q->used) {
      DEBUGF("Skipping quad %s %p %p %p", quad_op_name(q->op), (void *)q->q1,
             (void *)q->q2, (void *)q->q3);
      continue;
    }

    switch (q->op) {
      case QUAD_OP_ADD:
        fprintf(args->file,
                "%s%s_add(" TEMP "%d, " TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = " TEMP "%d + " TEMP "%d\n",
                ARG_3);
        break;

      case QUAD_OP_SUB:
        fprintf(args->file,
                "%s%s_sub(" TEMP "%d, " TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = " TEMP "%d - " TEMP "%d\n",
                ARG_3);
        break;

      case QUAD_OP_MUL:
        fprintf(args->file,
                "%s%s_mul(" TEMP "%d, " TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = " TEMP "%d * " TEMP "%d\n",
                ARG_3);
        break;

      case QUAD_OP_DIV:
        fprintf(args->file,
                "%s%s_div(" TEMP "%d, " TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = " TEMP "%d / " TEMP "%d\n",
                ARG_3);
        break;

      case QUAD_OP_ASSIGN:
        fprintf(args->file,
                "%s%s_set(" TEMP "%d, " TEMP "%d, %s); // " TEMP "%d = " TEMP
                "%d\n",
                ARG_2);
        break;

      case QUAD_OP_SQRT:
        fprintf(args->file,
                "%s%s_sqrt(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = sqrt(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_NEG:
        fprintf(args->file,
                "%s%s_neg(" TEMP "%d, " TEMP "%d, %s); // " TEMP "%d = -" TEMP
                "%d\n",
                ARG_2);
        break;

      case QUAD_OP_ABS:
        fprintf(args->file,
                "%s%s_abs(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = abs(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_EXP:
        fprintf(args->file,
                "%s%s_exp(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = exp(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_LOG:
        fprintf(args->file,
                "%s%s_log(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = log(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_POW:
        fprintf(args->file,
                "%s%s_pow(" TEMP "%d, " TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = pow(" TEMP "%d, " TEMP "%d)\n",
                ARG_3);
        break;

      case QUAD_OP_SIN:
        fprintf(args->file,
                "%s%s_sin(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = sin(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_COS:
        fprintf(args->file,
                "%s%s_cos(" TEMP "%d, " TEMP "%d, %s); // " TEMP
                "%d = cos(" TEMP "%d)\n",
                ARG_2);
        break;

      case QUAD_OP_INCR:
        fprintf(args->file,
                "%s%s_add_ui(" TEMP "%d, " TEMP "%d, 1, %s); // " TEMP "%d++\n",
                ARG_1);
        break;

      case QUAD_OP_DECR:
        fprintf(args->file,
                "%s%s_sub_ui(" TEMP "%d, " TEMP "%d, 1, %s); // " TEMP "%d--\n",
                ARG_1);
        break;

      case QUAD_LABEL:
        fprintf(args->file, "c2mp_label%d:\n", q->q1->number);
        break;

      case QUAD_GOTO:
        fprintf(args->file, "%sgoto c2mp_label%d;\n", indent, q->q1->number);
        break;

      case QUAD_NOOP:
        fprintf(args->file, "%s// Unsupported OP\n", indent);
        break;
    }
  }
}

void gencode_clear(gencode_args_t *args, symbol_t *symbol_table) {
  symbol_t *s;
  char *indent = "  ";
  char *lib    = gencode_lib_name(args->lib);

  if (symbol_table) {
    INFO("Generating the clear section");
  }

  for (s = symbol_table; s; s = s->next) {
    if (s->modified && s->name && !s->replaced) {
      symbol_t *tmp = s;
      while (tmp->alias)
        tmp = tmp->alias;
      if (tmp->type == SYM_UNKNOWN)
        WARNF("Could not infer type for symbol %p, assuming decimal",
              (void *)tmp);
      switch (tmp->type) {
        case SYM_UNKNOWN:
        case SYM_DECIMAL:
          if (tmp->declared)
            fprintf(args->file, "%sdouble ", indent);
          else
            fprintf(args->file, "%s", indent);
          fprintf(args->file, "%s = %s_get_d%s(" TEMP "%d, %s);\n", s->name,
                  lib, (args->lib == LIB_MPC) ? "c" : "", tmp->number,
                  args->rounding);
          break;

        case SYM_INTEGER:
          FATAL("Integers are not supported");
          break;

        case SYM_BOOLEAN:
          if (tmp->declared)
            fprintf(args->file, "%sbool ", indent);
          else
            fprintf(args->file, "%s", indent);
          fprintf(args->file, "%s = " BOOL "%d;\n", s->name, tmp->number);
          break;

        case SYM_LABEL:
          break;
      }
    }
  }

  if (symbol_table) {
    fprintf(args->file, "\n%s// free memory of all variables that we used\n",
            indent);
  }

  for (s = symbol_table; s; s = s->next) {
    if (s->alias)
      continue;
    fprintf(args->file, "%s%s_clear(" TEMP "%d);\n", indent, lib, s->number);
  }
}
