#ifndef GENCODE_H
#define GENCODE_H

#include "quad.h"
#include "symbol.h"
#include <stdio.h>

typedef enum gencode_lib_e { LIB_MPC, LIB_MPFR } gencode_lib_t;

typedef struct gencode_args_s {
  gencode_lib_t lib;
  uint32_t precision;
  char *rounding;
  FILE *file;
} gencode_args_t;

void gencode_init(gencode_args_t *args, symbol_t *symbol_table);
void gencode_assign(gencode_args_t *args, symbol_t *symbol_table);
void gencode_operations(gencode_args_t *args, op_list_t *list);
void gencode_clear(gencode_args_t *args, symbol_t *symbol_table);

#endif
