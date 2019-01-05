#include <assert.h>
#include <stdlib.h>

#include "statement.h"

stmt_t *stmt_alloc(stmt_type_t type) {
  stmt_t *stmt = malloc(sizeof(stmt_t));
  stmt->type = type;
  stmt->next = NULL;
  return stmt;
}

stmt_t *stmt_new_expr(ast_node_t *node) {
  assert(node);
  stmt_t *stmt = stmt_alloc(STMT_EXPR);
  stmt->c.expr = node;
  return stmt;
}

stmt_t *stmt_new_block(stmt_t *content) {
  stmt_t *stmt = stmt_alloc(STMT_BLOCK);
  stmt->c.block = content;
  return stmt;
}

stmt_t *stmt_new_cond(ast_node_t *condition, stmt_t *body, stmt_t *else_body) {
  assert(condition);
  stmt_t *stmt = stmt_alloc(STMT_COND);
  stmt->c.cond.condition = condition;
  stmt->c.cond.body = body;
  stmt->c.cond.else_body = else_body;
  return stmt;
}

stmt_t *stmt_new_loop(stmt_t *init, ast_node_t *cond, stmt_t *end, stmt_t *body) {
  stmt_t *stmt = stmt_alloc(STMT_LOOP);
  stmt->c.loop.initializers = init;
  stmt->c.loop.condition = cond;
  stmt->c.loop.end = end;
  stmt->c.loop.body = body;
  return stmt;
}

stmt_t *stmt_new_break(void) {
  return stmt_alloc(STMT_BREAK);
}

stmt_t *stmt_new_continue(void) {
  return stmt_alloc(STMT_CONTINUE);
}

stmt_t *stmt_new_return(ast_node_t *retval) {
  stmt_t *stmt = stmt_alloc(STMT_RETURN);
  stmt->c.retval = retval;
  return stmt;
}

void stmt_concat(stmt_t **head, stmt_t *tail) {
  stmt_t **tmp = head;
  while (*tmp)
    tmp = &(*tmp)->next;
  *tmp = tail;
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
    // ast_display(list->node);
    list = list->next;
  }
}

void stmt_gen_quad(stmt_t *list, symbol_t **table, op_list_t **ops) {
  (void)table;
  (void)ops;
  while (list) {
    // ast_gen_quad(list->node, table, ops);
    list = list->next;
  }
}
