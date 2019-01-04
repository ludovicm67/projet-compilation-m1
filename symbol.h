#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <stdint.h>

typedef enum symbol_type_e {
  SYM_UNKNOWN,
  SYM_DECIMAL,
  SYM_INTEGER,
  SYM_BOOLEAN,
} symbol_type_t;

typedef struct symbol_s {
  uint32_t number; // number of the variable (will be used at code generation)
  symbol_type_t type;
  char *name;
  bool external;
  bool readBeforeModified; // value has been read
  bool modified;           // value has been changed in the pragma block
  bool hasValue;           // if this symbol contains a value or not
  union {
    double decimal;
    int32_t integer;
    bool boolean;
  } value;
  struct symbol_s *next;
} symbol_t;

symbol_t *symbol_new(symbol_type_t type, char *name, bool external);
symbol_t *symbol_add(symbol_t **symbol, symbol_type_t type, char *name, bool external);
void symbol_set_type(symbol_t *, symbol_type_t);
void symbol_set_decimal(symbol_t *, double value);
void symbol_set_integer(symbol_t *, int value);
void symbol_set_boolean(symbol_t *, bool value);
symbol_t *symbol_lookup(symbol_t **symbol, char *name);
void symbol_delete(symbol_t *symbol);

#endif
