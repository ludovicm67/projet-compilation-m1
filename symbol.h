/**
 * @file symbol.h
 *
 * Manage all symbols
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Supported types for a symbol
 */
typedef enum symbol_type_e {
  SYM_UNKNOWN,
  SYM_DECIMAL,
  SYM_INTEGER,
  SYM_BOOLEAN,
  SYM_LABEL,
} symbol_type_t;

/**
 * Represents a symbol
 */
typedef struct symbol_s {
  uint32_t number; // number of the variable (will be used at code generation)
  symbol_type_t type;
  char *name;
  bool declared;
  // TODO(sandhose): store the symbol scope/depth
  bool readBeforeModified; // value has been read
  bool modified;           // value has been changed in the pragma block
  bool hasValue;           // if this symbol contains a value or not
  bool replaced;           // if this symbol was replaced by another
  bool assigned;
  bool used;
  union {
    double decimal;
    int32_t integer;
    bool boolean;
  } value;
  struct op_s *op;
  struct symbol_s *alias;
  struct symbol_s *next;
} symbol_t;

/**
 * Generate a new symbol
 *
 * @param[in] type the type of the symbol
 * @param[in] name the name of the symbol; may be set to `NULL` for temp vars
 * @param[in] declared if the symbol has been declared in the pragma block or not
 *
 * @return the new symbol
 */
symbol_t *symbol_new(symbol_type_t type, char *name, bool declared);

/**
 * Add a new symbol on the symbol table
 *
 * @param[in,out] symbol the symbol table
 * @param[in] type the symbol type
 * @param[in] name the symbol name
 * @param[in] declared if the symbol has been declared or not
 *
 * @return the new symbol
 */
symbol_t *symbol_add(symbol_t **symbol, symbol_type_t type, char *name, bool declared);

/**
 * Return a human-readable name for a given type
 *
 * @param[in] type the symbol type
 *
 * @return a statically allocated string
 */
char *symbol_type_name(symbol_type_t type);

/**
 * Set the type of a symbol
 *
 * @param[in,out] symbol the symbol that we need to change
 * @param[in] type the type that we want to set to this symbol
 */
void symbol_set_type(symbol_t *symbol, symbol_type_t type);

/**
 * Set a decimal value for a symbol
 *
 * @param[in,out] symbol the symbol that we want to set a value for
 * @param[in] value the decimal value that we want for the symbol
 */
void symbol_set_decimal(symbol_t *symbol, double value);

/**
 * Set a integer value for a symbol
 *
 * @param[in,out] symbol the symbol that we want to set a value for
 * @param[in] value the integer value that we want for the symbol
 */
void symbol_set_integer(symbol_t *symbol, int value);

/**
 * Set a boolean value for a symbol
 *
 * @param[in,out] symbol the symbol that we want to set a value for
 * @param[in] value the boolean value that we want for the symbol
 */
void symbol_set_boolean(symbol_t *symbol, bool value);

/**
 * Compare the values of two symbols, if they both have one
 */
bool symbol_compare_values(symbol_t *x, symbol_t *y);

/**
 * Lookup for a symbol using a name
 *
 * @param[in,out] symbol the symbol table in wich we want to look for
 * @param[in] name the name of the symbol we are looking for
 *
 * @return the symbol having the specified name, null if not found
 */
symbol_t *symbol_lookup(symbol_t **symbol, char *name);

/**
 * Delete a symbol table
 *
 * @param[in] symbol remove all symbols from the table from this symbol
 */
void symbol_delete(symbol_t *symbol);

#endif
