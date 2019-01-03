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

stmt_t *stmt_push(stmt_t *list, ast_node_t *node) {
  stmt_t *statement = stmt_new(node);
  statement->next = list;
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
