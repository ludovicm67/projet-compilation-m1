#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symbol_t *symbol_new(char *name, bool external, double value) {
  symbol_t *symbol = malloc(sizeof(symbol_t));
  symbol->number = 0;
  symbol->name = name;
  symbol->external = external;
  symbol->modified = false;
  symbol->value = value;
  symbol->next = NULL;
  return symbol;
}

symbol_t *symbol_add(symbol_t **symbol, char *name, bool external,
                     double value) {
  symbol_t *tmp;

  tmp = symbol_new(name, external, value);
  tmp->next = *symbol;

  *symbol = tmp;

  return tmp;
}

symbol_t *symbol_lookup(symbol_t **symbol, char *name) {
  symbol_t *tmp = *symbol;

  // check if symbol already exists
  for (tmp = *symbol; tmp; tmp = tmp->next) {
    if (tmp->name && !strcmp(tmp->name, name)) {
      return tmp;
    }
  }

  // if not add it (assume it is external)
  return symbol_add(symbol, name, true, 0);
}

void symbol_delete(symbol_t *symbol) {
  if (!symbol)
    return;
  symbol_delete(symbol->next);
  free(symbol);
}
