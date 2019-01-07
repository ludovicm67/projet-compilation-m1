/**
 * @file optim.h
 *
 * Do some optimizations on quads
 */

#ifndef OPTIM_H
#define OPTIM_H

#include "quad.h"

/**
 * Remove useless addtions with zero
 *
 * @param[in] quad the quad to optimize
 */
void optim_add_zero(op_t *quad);

/**
 * Remove useless multiplications with one
 *
 * @param[in] quad the quad to optimize
 */
void optim_mul_one(op_t *quad);

/**
 * Do some arithmical optimizations on a list of quads
 *
 * @param[in] quad the quad to optimize
 */
void optim_arith(op_list_t * list);

void optim_do(symbol_t *, op_list_t *, uint8_t);

#endif
