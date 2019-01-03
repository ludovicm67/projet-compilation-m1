#ifndef GENCODE_H
#define GENCODE_H

#include "quad.h"
#include "symbol.h"

void gencode_init(symbol_t *symbol_table, uint32_t precision);
void gencode_assign(symbol_t *symbol_table);
void gencode_operations(op_list_t *list);
void gencode_clear(symbol_t *symbol_table);

#endif
