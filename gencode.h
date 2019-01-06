/**
 * @file gencode.h
 *
 * Generate differents parts of code.
 */

#ifndef GENCODE_H
#define GENCODE_H

#include <stdio.h>

#include "quad.h"
#include "symbol.h"

/**
 * The different supported libraries
 */
typedef enum gencode_lib_e { LIB_MPC, LIB_MPFR } gencode_lib_t;

/**
 * Arguments for gencode
 */
typedef struct gencode_args_s {
  gencode_lib_t lib;
  uint32_t precision;
  char *rounding;
  FILE *file;
} gencode_args_t;

/**
 * Generate code for MP variables initialization
 *
 * @param[in] args gencode arguments
 * @param[in] symbol_table the symbol table
 */
void gencode_init(gencode_args_t *args, symbol_t *symbol_table);

/**
 * Generate code for MP variables assignations
 *
 * @param[in] args gencode arguments
 * @param[in] symbol_table the symbol table
 */
void gencode_assign(gencode_args_t *args, symbol_t *symbol_table);


/**
 * Generate code for MP operations
 *
 * @param[in] args gencode arguments
 * @param[in] list list of quads
 */
void gencode_operations(gencode_args_t *args, op_list_t *list);

/**
 * Generate code for MP variables clear
 *
 * @param[in] args gencode arguments
 * @param[in] symbol_table the symbol table
 */
void gencode_clear(gencode_args_t *args, symbol_t *symbol_table);

#endif
