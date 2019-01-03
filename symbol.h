#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct symbol_s {
  uint32_t number; // number of the variable (will be used at code generation)
  char *name;
  bool external;
  bool modified; // value has been changed in the pragma block
  bool hasValue; // if this symbol contains a value or not
  double value;
  struct symbol_s *next;
} symbol_t;

symbol_t *symbol_new(char *name, bool external, bool hasValue, double value);
symbol_t *symbol_add(symbol_t **symbol, char *name, bool external,
                     bool hasValue, double value);
symbol_t *symbol_lookup(symbol_t **symbol, char *name);
void symbol_delete(symbol_t *symbol);

#endif
