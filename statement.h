#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdint.h>

typedef enum stmt_type_e {
  STMT_EXPR,
  STMT_BLOCK,
  STMT_COND,
  STMT_LOOP,
  STMT_BREAK,
  STMT_CONTINUE,
  STMT_RETURN,
} stmt_type_t;

typedef struct stmt_s stmt_t;

#include "ast.h"

struct stmt_s {
  stmt_type_t type; ///< The type of statement
  union {
    ast_node_t *expr; ///< Content of a `STMT_EXPR`.

    stmt_t *block; ///< Content of a `STMT_BLOCK`.

    struct {
      ast_node_t *condition; ///< The condition itself.
      stmt_t *body; ///< The `then` part of the condition.
                    ///< Can be empty.
      stmt_t *else_body; ///< The `else part of the condition.
                         ///< Can be empty.
    } cond; ///< Content of a `STMT_COND`.

    struct {
      stmt_t *initializers; ///< The initialization section of a `for` loop.
                            ///< Can be empty.
      ast_node_t *condition; ///< The condition part of the loop.
                             ///< Can be NULL for infinite loops.
      stmt_t *end; ///< The end part of a `for` loop.
                   ///< Can be empty.
      stmt_t *body; ///< The body of the loop.
                    ///< Can be empty.
    } loop; ///< Content of a `STMT_LOOP`.

    ast_node_t *retval; ///< Content of a `STMT_RETURN`.
                        ///< Can be `NULL`.
  } c;
  stmt_t *next;
};

stmt_t *stmt_new_expr(ast_node_t *);
stmt_t *stmt_new_block(stmt_t *content);
stmt_t *stmt_new_cond(ast_node_t *condition, stmt_t *body, stmt_t *else_body);
stmt_t *stmt_new_loop(stmt_t *init, ast_node_t *cond, stmt_t *end, stmt_t *body);
stmt_t *stmt_new_break(void);
stmt_t *stmt_new_continue(void);
stmt_t *stmt_new_return(ast_node_t *retval);

void stmt_concat(stmt_t **head, stmt_t *tail);
void stmt_delete(stmt_t *list);
uint8_t stmt_count(stmt_t *);
void stmt_display(stmt_t *);
void stmt_gen_quad(stmt_t *, symbol_t **table, op_list_t **ops);

#endif
