#ifndef QUAD_H
#define QUAD_H

#include "symbol.h"

typedef enum quad_op_e { QUAD_OP_ADD, QUAD_OP_SUB } quad_op_t;

typedef struct op_s {
  quad_op_t op;
  symbol_t *q1;
  symbol_t *q2;
  symbol_t *q3;
} op_t;

op_t *quad_new(quad_op_t op, symbol_t *q1, symbol_t *q2, symbol_t *q3);
void quad_free(op_t *q);

#endif