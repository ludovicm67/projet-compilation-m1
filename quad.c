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

void quad_delete(op_t *q) {
  if (!q)
    return;
  free(q);
}

op_list_t *quad_list_new(op_t *quad) {
  op_list_t *list = malloc(sizeof(op_list_t));
  list->quad = quad;
  list->next = NULL;
  return list;
}

void quad_list_append(op_list_t *list, op_t *quad) {
  if (!list)
    return;
  while (list->next)
    list = list->next;
  list->next = quad_list_new(quad);
}

op_list_t *quad_list_concat(op_list_t *list1, op_list_t *list2) {
  if (!list2)
    return list1;
  if (!list1)
    return list2;
  op_list_t *list = quad_list_new(list1->quad);
  op_list_t *tmp = list;
  list1 = list1->next;
  while (list1 && list1->next) {
    tmp->next = quad_list_new(list1->quad);
    list1 = list1->next;
  }
  while (list2->next) {
    tmp->next = quad_list_new(list2->quad);
    list2 = list2->next;
  }
  return list;
}

void quad_list_delete(op_list_t *list) {
  if (!list)
    return;
  quad_list_delete(list->next);
  free(list);
}
