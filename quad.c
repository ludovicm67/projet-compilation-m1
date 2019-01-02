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

void quad_free(op_t *q) {
  if (!q)
    return;
  free(q);
}
