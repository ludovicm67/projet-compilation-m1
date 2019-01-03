#ifndef GENCODE_H
#define GENCODE_H

#include "symbol.h"

void gencode_init(symbol_t *symbol_table, uint32_t precision);
void gencode_assign(symbol_t *symbol_table);
void gencode_clear(symbol_t *symbol_table);

#endif
