#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdint.h>

#include "ast.h"

typedef struct stmt_s stmt_t;
struct stmt_s {
  ast_node_t *node;
  stmt_t *next;
};

stmt_t *stmt_new(ast_node_t *);
stmt_t *stmt_push(stmt_t *list, ast_node_t *);
void stmt_delete(stmt_t *list);
uint8_t stmt_count(stmt_t *);

#endif
