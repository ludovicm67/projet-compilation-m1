#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

typedef struct symbol_s {
  char *name;
  bool isMutable;
  int value;
  struct symbol_s *next;
} symbol_t;

symbol_t *symbol_new(char *name, bool isMutable, int value);

#endif
