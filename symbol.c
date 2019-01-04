#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

symbol_t *symbol_new(symbol_type_t type, char *name, bool external) {
  symbol_t *symbol = malloc(sizeof(symbol_t));
  symbol->number = 0;
  symbol->type = type;
  symbol->name = name;
  symbol->external = external;
  symbol->readBeforeModified = false;
  symbol->modified = false;
  symbol->hasValue = false;
  symbol->next = NULL;
  return symbol;
}

symbol_t *symbol_add(symbol_t **symbol, symbol_type_t type, char *name, bool external) {
  symbol_t *tmp;

  tmp = symbol_new(type, name, external);
  tmp->next = *symbol;

  *symbol = tmp;

  return tmp;
}

void symbol_set_type(symbol_t *symbol, symbol_type_t type) {
  assert(symbol->type == type || symbol->type == SYM_UNKNOWN);
  symbol->type = type;
}

void symbol_set_decimal(symbol_t *symbol, double value) {
  symbol_set_type(symbol, SYM_DECIMAL);
  symbol->hasValue = true;
  symbol->value.decimal = value;
}

void symbol_set_integer(symbol_t *symbol, int value) {
  symbol_set_type(symbol, SYM_INTEGER);
  symbol->hasValue = true;
  symbol->value.integer = value;
}

void symbol_set_boolean(symbol_t *symbol, bool value) {
  symbol_set_type(symbol, SYM_BOOLEAN);
  symbol->hasValue = true;
  symbol->value.boolean = value;
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
  return symbol_add(symbol, SYM_UNKNOWN, name, true);
}

void symbol_delete(symbol_t *symbol) {
  if (!symbol)
    return;
  symbol_delete(symbol->next);
  free(symbol);
}
