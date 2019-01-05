#include "optim.h"
#include <stdlib.h>

void optim_add_zero(op_t *quad) {
  if (quad->op == QUAD_OP_ADD || quad->op == QUAD_OP_SUB) {

    if (quad->q2->hasValue) {
      switch (quad->q2->type) {
        case SYM_DECIMAL:
          if (quad->q2->value.decimal == 0) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q2 = quad->q3;
            quad->q3 = NULL;
            return;
          }
          break;
        case SYM_INTEGER:
          if (quad->q2->value.integer == 0) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q2 = quad->q3;
            quad->q3 = NULL;
            return;
          }
          break;
        default:
          break;
      }
    }

    if (quad->q3->hasValue) {
      switch (quad->q3->type) {
        case SYM_DECIMAL:
          if (quad->q3->value.decimal == 0) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q3 = NULL;
            return;
          }
          break;
        case SYM_INTEGER:
          if (quad->q3->value.integer == 0) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q3 = NULL;
            return;
          }
          break;
        default:
          break;
      }
    }
  }
}

void optim_mul_one(op_t *quad) {
  if (quad->op == QUAD_OP_MUL || quad->op == QUAD_OP_DIV) {

    if (quad->q2->hasValue) {
      switch (quad->q2->type) {
        case SYM_DECIMAL:
          if (quad->q2->value.decimal == 1) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q2 = quad->q3;
            quad->q3 = NULL;
            return;
          }
          break;
        case SYM_INTEGER:
          if (quad->q2->value.integer == 1) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q2 = quad->q3;
            quad->q3 = NULL;
            return;
          }
          break;
        default:
          break;
      }
    }

    if (quad->q3->hasValue) {
      switch (quad->q3->type) {
        case SYM_DECIMAL:
          if (quad->q3->value.decimal == 1) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q3 = NULL;
            return;
          }
          break;
        case SYM_INTEGER:
          if (quad->q3->value.integer == 1) {
            quad->op = QUAD_OP_ASSIGN;
            quad->q3 = NULL;
            return;
          }
          break;
        default:
          break;
      }
    }
  }
}

void optim_arith(op_list_t *list) {
  op_t *quad;

  while (list) {
    quad = list->quad;

    optim_add_zero(quad);
    optim_mul_one(quad);

    list = list->next;
  }
}
