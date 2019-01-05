#ifndef OPTIM_H
#define OPTIM_H

#include "quad.h"

void optim_add_zero(op_t *quad);
void optim_mul_one(op_t *quad);
void optim_arith(op_list_t * list);

#endif
