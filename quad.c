#include "quad.h"
#include <stdio.h>
#include <stdlib.h>

op_t *quad_new(quad_op_t op, symbol_t *q1, symbol_t *q2, symbol_t *q3) {
  op_t *quad = malloc(sizeof(op_t));
  quad->op = op;
  quad->q1 = q1;
  quad->q2 = q2;
  quad->q3 = q3;
  return quad;
}

void quad_delete(op_t *q) { free(q); }

op_list_t *quad_list_new(op_t *quad) {
  op_list_t *list = malloc(sizeof(op_list_t));
  list->quad = quad;
  list->next = NULL;
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
