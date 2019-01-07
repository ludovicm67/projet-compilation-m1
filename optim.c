#include <stdlib.h>

#include "optim.h"
#include "util.h"

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

bool optim_apply_assign(op_list_t *ops) {
  bool changed = false;
  for (; ops; ops = ops->next) {
    op_t *quad = ops->quad;
    if (quad->op == QUAD_OP_ASSIGN) {
      quad->q1->alias = quad->q2;
      changed         = true;
    }
  }
  return changed;
}

bool optim_merge_symbols(symbol_t *symbol) {
  bool changed = false;
  for (symbol_t *tmp = symbol; tmp; tmp = tmp->next) {
    if (tmp->alias)
      continue;

    for (symbol_t *tmp2 = tmp->next; tmp2; tmp2 = tmp2->next) {
      if (tmp2->alias)
        continue;

      if (symbol_compare_values(tmp, tmp2)) {
        tmp2->alias = tmp;
        changed     = true;
      }
    }
  }
  return changed;
}

void optim_graph(op_list_t *list) {
  for (; list; list = list->next) {
    list->quad->used = false;
    if (list->quad->q1->alias)
      continue;
    if (list->quad->q1->op)
      FATAL("Symbol has two setters");
    list->quad->q1->op = list->quad;
  }
}

void optim_mark_used(op_t *op) {
  if (!op || op->used)
    return;
  op->used = true;

  if (op->q2)
    optim_mark_used(op->q2->op);

  if (op->q3)
    optim_mark_used(op->q3->op);
}

void optim_dead_code(symbol_t *symbol) {
  for (; symbol; symbol = symbol->next) {
    if (symbol->name && symbol->type != SYM_LABEL && symbol->modified &&
        !symbol->replaced) {
      symbol_t *target = symbol;
      while (target->alias)
        target = target->alias;
      optim_mark_used(target->op);
    }
  }
}

void optim_follow_aliases(op_list_t *list) {
  for (; list; list = list->next) {
    op_t *op = list->quad;

    if (op->q1)
      while (op->q1->alias)
        op->q1 = op->q1->alias;
    if (op->q2)
      while (op->q2->alias)
        op->q2 = op->q2->alias;
    if (op->q3)
      while (op->q3->alias)
        op->q3 = op->q3->alias;
  }
}

void optim_do(symbol_t *table, op_list_t *list, uint8_t max_iter) {
  optim_arith(list);
  optim_apply_assign(list); // Alias assignement
  for (uint8_t i = 0; i < max_iter; i++) {
    if (!optim_merge_symbols(table)) {
      break;
    }
  }
  optim_graph(list);
  optim_follow_aliases(list);
  optim_dead_code(table);
}
