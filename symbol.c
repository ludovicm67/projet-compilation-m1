#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symbol_t *symbol_new(char *name, bool isMutable, int value) {
  symbol_t *symbol = malloc(sizeof(symbol_t));
  symbol->name = name;
  symbol->isMutable = isMutable;
  symbol->value = value;
  symbol->next = NULL;
  return symbol;
}

symbol_t *symbol_add(symbol_t *symbol, char *name, bool isMutable, int value) {
  symbol_t *tmp;

  if (!symbol)
    return symbol_new(name, isMutable, value);

  // check if symbol already exists
  for (tmp = symbol; tmp; tmp = tmp->next) {
    if (!strcmp(tmp->name, name)) {
      if (!tmp->isMutable) {
        fprintf(stderr, "cannot change value of '%s'\n", name);
        exit(EXIT_FAILURE);
      }
      tmp->value = value;
      tmp->isMutable = isMutable;
      return symbol;
    }
  }

  // if not add it
  tmp = symbol_new(name, isMutable, value);
  tmp->next = symbol;

  return tmp;
}
