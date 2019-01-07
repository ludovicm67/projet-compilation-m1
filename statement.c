#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statement.h"
#include "util.h"

static char *stmt_type_map[] = {
    [STMT_EXPR] = "expression",   [STMT_BLOCK] = "block",
    [STMT_DECL] = "declaration",  [STMT_COND] = "if",
    [STMT_LOOP] = "loop",         [STMT_BREAK] = "break",
    [STMT_CONTINUE] = "continue", [STMT_RETURN] = "return",
};

stmt_t *stmt_alloc(stmt_type_t type) {
  stmt_t *stmt = malloc(sizeof(stmt_t));
  stmt->type   = type;
  stmt->next   = NULL;
  return stmt;
}

stmt_t *stmt_new_expr(ast_node_t *node) {
  assert(node);
  stmt_t *stmt = stmt_alloc(STMT_EXPR);
  stmt->c.expr = node;
  return stmt;
}

stmt_t *stmt_new_block(stmt_t *content) {
  stmt_t *stmt  = stmt_alloc(STMT_BLOCK);
  stmt->c.block = content;
  return stmt;
}

stmt_t *stmt_new_decl(stmt_decl_type_t type, char *symbol, ast_node_t *value) {
  stmt_t *stmt      = stmt_alloc(STMT_DECL);
  stmt->c.decl.type = type;
  stmt->c.decl.lval = strdup(symbol);
  stmt->c.decl.rval = value;
  return stmt;
}

stmt_t *stmt_new_cond(ast_node_t *condition, stmt_t *body, stmt_t *else_body) {
  assert(condition);
  stmt_t *stmt           = stmt_alloc(STMT_COND);
  stmt->c.cond.condition = condition;
  stmt->c.cond.body      = body;
  stmt->c.cond.else_body = else_body;
  return stmt;
}

stmt_t *stmt_new_loop(stmt_t *init, ast_node_t *cond, stmt_t *end,
                      stmt_t *body) {
  stmt_t *stmt              = stmt_alloc(STMT_LOOP);
  stmt->c.loop.initializers = init;
  stmt->c.loop.condition    = cond;
  stmt->c.loop.end          = end;
  stmt->c.loop.body         = body;
  return stmt;
}

stmt_t *stmt_new_break(void) { return stmt_alloc(STMT_BREAK); }

stmt_t *stmt_new_continue(void) { return stmt_alloc(STMT_CONTINUE); }

stmt_t *stmt_new_return(ast_node_t *retval) {
  stmt_t *stmt   = stmt_alloc(STMT_RETURN);
  stmt->c.retval = retval;
  return stmt;
}

stmt_t *stmt_decl_from_assign(stmt_decl_type_t type, ast_node_t *assign) {
  assert(assign->type == NODE_ASSIGN);
  stmt_t *stmt      = stmt_alloc(STMT_DECL);
  stmt->c.decl.type = type;
  stmt->c.decl.lval = assign->c.assign.lval;
  stmt->c.decl.rval = assign->c.assign.rval;
  free(assign);
  return stmt;
}

void stmt_concat(stmt_t **head, stmt_t *tail) {
  stmt_t **tmp = head;
  while (*tmp)
    tmp = &(*tmp)->next;
  *tmp = tail;
}

void stmt_delete(stmt_t *stmt) {
  stmt_t *tmp = stmt;
  while (stmt) {
    switch (stmt->type) {
      case STMT_EXPR:
        ast_delete(stmt->c.expr);
        break;

      case STMT_BLOCK:
        stmt_delete(stmt->c.block);
        break;

      case STMT_COND:
        ast_delete(stmt->c.cond.condition);
        stmt_delete(stmt->c.cond.body);
        stmt_delete(stmt->c.cond.else_body);
        break;

      case STMT_LOOP:
        stmt_delete(stmt->c.loop.initializers);

        if (stmt->c.loop.condition)
          ast_delete(stmt->c.loop.condition);

        stmt_delete(stmt->c.loop.end);
        stmt_delete(stmt->c.loop.body);
        break;

      case STMT_DECL:
        free(stmt->c.decl.lval);
        ast_delete(stmt->c.decl.rval);
        break;

      case STMT_BREAK:
      case STMT_CONTINUE:
        break;

      case STMT_RETURN:
        if (stmt->c.retval)
          ast_delete(stmt->c.retval);
        break;
    }

    tmp = stmt->next;
    free(stmt);
    stmt = tmp;
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

void ast_display_i(ast_node_t *, uint8_t);

void stmt_display_i(stmt_t *stmt, uint8_t i) {
  while (stmt) {
    switch (stmt->type) {
      case STMT_EXPR:
        ast_display_i(stmt->c.expr, i);
        break;

      case STMT_BLOCK:
        indent(i);
        fprintf(stdout, "Block\n");
        stmt_display_i(stmt->c.block, i + 1);
        break;

      case STMT_DECL:
        indent(i);
        if (stmt->c.decl.rval) {
          fprintf(stdout, "Declaration %d %s =\n", stmt->c.decl.type,
                  stmt->c.decl.lval);
          ast_display_i(stmt->c.decl.rval, i + 1);
        } else {
          fprintf(stdout, "Declaration %d %s\n", stmt->c.decl.type,
                  stmt->c.decl.lval);
        }
        break;

      case STMT_COND:
        indent(i);
        fprintf(stdout, "If\n");
        ast_display_i(stmt->c.cond.condition, i + 1);
        indent(i);
        fprintf(stdout, "Then\n");
        stmt_display_i(stmt->c.cond.body, i + 1);

        if (stmt->c.cond.else_body) {
          indent(i);
          fprintf(stdout, "Else\n");
          stmt_display_i(stmt->c.cond.else_body, i + 1);
        }
        break;

      case STMT_LOOP:
        fprintf(stdout, "Loop\n");
        if (stmt->c.loop.initializers) {
          indent(i);
          fprintf(stdout, "Init\n");
          stmt_display_i(stmt->c.loop.initializers, i + 1);
        }

        if (stmt->c.loop.condition) {
          indent(i);
          fprintf(stdout, "Condition\n");
          ast_display_i(stmt->c.loop.condition, i + 1);
        }

        indent(i);
        fprintf(stdout, "Body\n");
        stmt_display_i(stmt->c.loop.body, i + 1);

        if (stmt->c.loop.end) {
          indent(i);
          fprintf(stdout, "End\n");
          stmt_display_i(stmt->c.loop.end, i + 1);
        }
        break;

      case STMT_BREAK:
        printf("Break\n");
        break;

      case STMT_CONTINUE:
        printf("Continue\n");
        break;

      case STMT_RETURN:
        printf("Return\n");

        if (stmt->c.retval)
          ast_display_i(stmt->c.retval, i + 1);

        break;
    }
    stmt = stmt->next;
  }
}

void stmt_display(stmt_t *list) { stmt_display_i(list, 0); }

void stmt_gen_quad(stmt_t *stmt, symbol_t **table, op_list_t **ops) {
  while (stmt) {
    switch (stmt->type) {
      case STMT_EXPR:
        DEBUGF("Generating quad for expression %p", (void *)stmt->c.expr);
        ast_gen_quad(stmt->c.expr, table, ops);
        break;

      case STMT_BLOCK:
        DEBUGF("Generating quad for block %p", (void *)stmt->c.block);
        stmt_gen_quad(stmt->c.block, table, ops);
        break;

      case STMT_DECL: {
        symbol_type_t type;
        switch (stmt->c.decl.type) {
          case TYPE_INT:
            type = SYM_INTEGER;
            break;

          case TYPE_DOUBLE:
          case TYPE_FLOAT:
            type = SYM_DECIMAL;
            break;

          case TYPE_BOOL:
            type = SYM_BOOLEAN;
            break;

          case TYPE_COMPLEX:
            type = SYM_UNKNOWN;
            break;
        }

        symbol_t *dest = symbol_add(table, type, stmt->c.decl.lval, true);
        DEBUGF("Declaration of `%s'", stmt->c.decl.lval);
        if (stmt->c.decl.rval) {
          dest->modified = true;
          symbol_t *temp = ast_gen_quad(stmt->c.decl.rval, table, ops);
          op_t *quad     = quad_new(QUAD_OP_ASSIGN, dest, temp, NULL);
          quad_list_append(ops, quad);
        }

        break;
      }

      case STMT_COND:
      case STMT_LOOP:
      case STMT_BREAK:
      case STMT_CONTINUE:
      case STMT_RETURN:
        // TODO(sandhose): Generate code
        FATALF("Unsupported `%s' statement", stmt_type_map[stmt->type]);
        break;
    }
    stmt = stmt->next;
  }
}
