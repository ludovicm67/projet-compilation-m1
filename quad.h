/**
 * @file quad.h
 *
 * Define structures needed for quads and some operations on them
 */

#ifndef QUAD_H
#define QUAD_H

#include "symbol.h"

/**
 * The diffrent operations for a quad
 */
typedef enum quad_op_e {
  QUAD_OP_ADD,    ///< `x + y`
  QUAD_OP_SUB,    ///< `x - y`
  QUAD_OP_MUL,    ///< `x * y`
  QUAD_OP_DIV,    ///< `x / y`
  QUAD_OP_ASSIGN, ///< `x = y`
  QUAD_OP_SQRT,   ///< `sqrt(x)`
  QUAD_OP_NEG,    ///< `-x`
  QUAD_OP_ABS,    ///< `abs(x)`
  QUAD_OP_EXP,    ///< `exp(x)`
  QUAD_OP_LOG,    ///< `log(x)`
  QUAD_OP_POW,    ///< `pow(x)`
  QUAD_OP_SIN,    ///< `sin(x)`
  QUAD_OP_COS,    ///< `cos(x)`
  QUAD_OP_INCR,   ///< `x++`
  QUAD_OP_DECR,   ///< `x--`
  QUAD_NOOP,      ///< unsupported operation
} quad_op_t;

/**
 * Type which represents a quad
 */
typedef struct op_s {
  quad_op_t op;
  symbol_t *q1;
  symbol_t *q2;
  symbol_t *q3;
} op_t;

/**
 * List of quads
 */
typedef struct op_list_s {
  op_t *quad;
  struct op_list_s *next;
} op_list_t;

/**
 * Generate a new quad
 *
 * @param[in] op quad operation
 * @param[in] q1 destination symbol
 * @param[in] q2 first symbol for the operation
 * @param[in] q3 second symbol for the operation
 *
 * @return the new quad
 */
op_t *quad_new(quad_op_t op, symbol_t *q1, symbol_t *q2, symbol_t *q3);

/**
 * Delete a quad
 *
 * @param[in] q the quad to delete
 */
void quad_delete(op_t *q);

/**
 * Generate a new list of quads
 *
 * @param[in] quad the first quad to insert on the list
 */
op_list_t *quad_list_new(op_t *quad);

/**
 * Append a quad on list of quad
 *
 * @param[in,out] list the list of quads
 * @param[in] quad the quad to insert at the end of the list
 */
void quad_list_append(op_list_t **list, op_t *quad);

/**
 * Concatenate a list of quads with an other one
 *
 * @param[in,out] list1 the list of quads, will be directly modified
 * @param[in] list2 the list of quads to append an the end of the first list
 */
void quad_list_concat(op_list_t **list1, op_list_t *list2);

/**
 * Delete a list of quad
 *
 * @param[in] list the list of quad to delete
 */
void quad_list_delete(op_list_t *list);

#endif
