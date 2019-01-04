#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdint.h>

typedef struct stmt_s stmt_t;

#include "ast.h"

struct stmt_s {
  ast_node_t *node;
  stmt_t *next;
};

stmt_t *stmt_new(ast_node_t *);
void stmt_concat(stmt_t **head, stmt_t *tail);
stmt_t *stmt_push(stmt_t *list, ast_node_t *);
void stmt_delete(stmt_t *list);
uint8_t stmt_count(stmt_t *);
void stmt_display(stmt_t *);
void stmt_gen_quad(stmt_t *, symbol_t **table, op_list_t **ops);

#endif
