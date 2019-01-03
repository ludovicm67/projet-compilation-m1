#include <assert.h>
#include <stdlib.h>

#include "statement.h"

stmt_t *stmt_new(ast_node_t *node) {
  assert(node);
  stmt_t *statement = malloc(sizeof(stmt_t));
  statement->node = node;
  statement->next = NULL;
  return statement;
}

void stmt_concat(stmt_t **head, stmt_t *tail) {
  stmt_t **tmp = head;
  while (*tmp)
    tmp = &(*tmp)->next;
  *tmp = tail;
}

stmt_t *stmt_push(stmt_t *list, ast_node_t *node) {
  stmt_t *statement = stmt_new(node);
  stmt_concat(&statement, list);
  return statement;
}

void stmt_delete(stmt_t *list) {
  stmt_t *tmp = list;
  while (list) {
    tmp = list->next;
    free(list);
    list = tmp;
  }
}

uint8_t stmt_count(stmt_t *list) {
  uint8_t i = 0;
  while (list) {
    list = list->next;
    i++;
  }
  return i;
}

void stmt_display(stmt_t *list) {
  while (list) {
    ast_display(list->node);
    list = list->next;
  }
}

void stmt_gen_quad(stmt_t *list, symbol_t **table, op_list_t **ops) {
  while (list) {
    ast_gen_quad(list->node, table, ops);
    list = list->next;
  }
}
