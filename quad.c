#include <stdio.h>
#include <stdlib.h>

#include "quad.h"
#include "util.h"

static char *names[] = {
    [QUAD_OP_ADD] = "ADD", [QUAD_OP_SUB] = "SUB",       [QUAD_OP_MUL] = "MUL",
    [QUAD_OP_DIV] = "DIV", [QUAD_OP_ASSIGN] = "ASSIGN", [QUAD_OP_SQRT] = "SQRT",
    [QUAD_OP_NEG] = "NEG", [QUAD_OP_ABS] = "ABS",       [QUAD_OP_EXP] = "EXP",
    [QUAD_OP_LOG] = "LOG", [QUAD_OP_POW] = "POW",       [QUAD_OP_SIN] = "SIN",
    [QUAD_OP_COS] = "COS", [QUAD_OP_INCR] = "INCR",     [QUAD_OP_DECR] = "DECR",
    [QUAD_NOOP] = "NOOP",
};

op_t *quad_new(quad_op_t op, symbol_t *q1, symbol_t *q2, symbol_t *q3) {
  DEBUGF("quad_new(%p <- %s %p %p)", (void *)q1, names[op], (void *)q2,
         (void *)q3);
  op_t *quad = malloc(sizeof(op_t));
  quad->op   = op;
  quad->q1   = q1;
  quad->q2   = q2;
  quad->q3   = q3;
  return quad;
}

void quad_delete(op_t *q) { free(q); }

op_list_t *quad_list_new(op_t *quad) {
  op_list_t *list = malloc(sizeof(op_list_t));
  list->quad      = quad;
  list->next      = NULL;
  return list;
}

void quad_list_append(op_list_t **list, op_t *quad) {
  quad_list_concat(list, quad_list_new(quad));
}

void quad_list_concat(op_list_t **list1, op_list_t *list2) {
  op_list_t **tmp = list1;

  while (*tmp)
    tmp = &(*tmp)->next;
  *tmp = list2;
}

void quad_list_delete(op_list_t *list) {
  while (list) {
    op_list_t *tmp = list->next;
    quad_delete(list->quad);
    free(list);
    list = tmp;
  }
}
