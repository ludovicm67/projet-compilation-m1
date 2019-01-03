#ifndef GENCODE_H
#define GENCODE_H

#include "quad.h"
#include "symbol.h"
#include <stdio.h>

void gencode_init(FILE *file, symbol_t *symbol_table, uint32_t precision);
void gencode_assign(FILE *file, symbol_t *symbol_table, char *rounding);
void gencode_operations(FILE *file, op_list_t *list, char *rounding);
void gencode_clear(FILE *file, symbol_t *symbol_table, char *rounding);

#endif
