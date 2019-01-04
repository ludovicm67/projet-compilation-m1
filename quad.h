#ifndef QUAD_H
#define QUAD_H

#include "symbol.h"

typedef enum quad_op_e {
  QUAD_OP_ADD,
  QUAD_OP_SUB,
  QUAD_OP_MUL,
  QUAD_OP_ASSIGN,
  QUAD_OP_SQRT,
  QUAD_OP_NEG,
  QUAD_NOOP,
} quad_op_t;

typedef struct op_s {
  quad_op_t op;
  symbol_t *q1;
  symbol_t *q2;
  symbol_t *q3;
} op_t;

typedef struct op_list_s {
  op_t *quad;
  struct op_list_s *next;
} op_list_t;

op_t *quad_new(quad_op_t op, symbol_t *q1, symbol_t *q2, symbol_t *q3);
void quad_delete(op_t *q);
op_list_t *quad_list_new(op_t *quad);
void quad_list_append(op_list_t **list, op_t *quad);
void quad_list_concat(op_list_t **list1, op_list_t *list2);
void quad_list_delete(op_list_t *list);

#endif
