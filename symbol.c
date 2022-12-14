#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "util.h"

symbol_t *symbol_new(symbol_type_t type, char *name, bool declared) {
  symbol_t *symbol           = malloc(sizeof(symbol_t));
  symbol->number             = 0;
  symbol->type               = type;
  symbol->name               = name;
  symbol->declared           = declared;
  symbol->readBeforeModified = false;
  symbol->modified           = false;
  symbol->replaced           = false;
  symbol->hasValue           = false;
  symbol->assigned           = false;
  symbol->used               = true;
  symbol->op                 = NULL;
  symbol->alias              = NULL;
  symbol->next               = NULL;
  return symbol;
}

symbol_t *symbol_add(symbol_t **symbol, symbol_type_t type, char *name,
                     bool declared) {
  symbol_t *tmp;

  tmp       = symbol_new(type, name, declared);
  tmp->next = *symbol;

  *symbol = tmp;

  return tmp;
}

static char *symbol_type_name_map[] = {
    [SYM_UNKNOWN] = "unknown", [SYM_DECIMAL] = "decimal",
    [SYM_INTEGER] = "integer", [SYM_BOOLEAN] = "boolean",
    [SYM_LABEL] = "label",
};

char *symbol_type_name(symbol_type_t type) {
  return symbol_type_name_map[type];
}

void symbol_set_type(symbol_t *symbol, symbol_type_t type) {
  assert(symbol->type == type || symbol->type == SYM_UNKNOWN);
  symbol->type = type;
}

void symbol_set_decimal(symbol_t *symbol, double value) {
  symbol_set_type(symbol, SYM_DECIMAL);
  symbol->hasValue      = true;
  symbol->value.decimal = value;
}

void symbol_set_integer(symbol_t *symbol, int value) {
  symbol_set_type(symbol, SYM_INTEGER);
  symbol->hasValue      = true;
  symbol->value.integer = value;
}

void symbol_set_boolean(symbol_t *symbol, bool value) {
  symbol_set_type(symbol, SYM_BOOLEAN);
  symbol->hasValue      = true;
  symbol->value.boolean = value;
}

bool symbol_compare_values(symbol_t *x, symbol_t *y) {
  return (x->type == y->type && x->hasValue && y->hasValue &&
          ((x->type == SYM_DECIMAL && x->value.decimal == y->value.decimal) ||
           (x->type == SYM_INTEGER && x->value.integer == y->value.integer) ||
           (x->type == SYM_BOOLEAN && x->value.boolean == y->value.boolean)));
}

symbol_t *symbol_lookup(symbol_t **symbol, char *name) {
  symbol_t *tmp = *symbol;

  // check if symbol already exists
  for (tmp = *symbol; tmp; tmp = tmp->next) {
    if (tmp->name && !strcmp(tmp->name, name)) {
      DEBUGF("Symbol `%s' found (%p)", name, (void *)tmp);
      return tmp;
    }
  }

  DEBUGF("Symbol `%s' not found", name);
  return NULL;
}

void symbol_delete(symbol_t *symbol) {
  while (symbol) {
    symbol_t *tmp = symbol->next;
    free(symbol);
    symbol = tmp;
  }
}
