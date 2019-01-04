#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

ast_node_t *ast_alloc(void) { return malloc(sizeof(ast_node_t)); }

ast_node_t *ast_new_unary(ast_unary_op_t type, ast_node_t *arg) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_UNARY;
  node->c.unary.type = type;
  node->c.unary.arg = arg;
  return node;
}

ast_node_t *ast_new_binary(ast_binary_op_t type, ast_node_t *left,
                           ast_node_t *right) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_BINARY;
  node->c.binary.type = type;
  node->c.binary.left = left;
  node->c.binary.right = right;
  return node;
}

ast_node_t *ast_new_assign(char *lval, ast_node_t *rval) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_ASSIGN;
  node->c.assign.lval = lval;
  node->c.assign.rval = rval;
  return node;
}

ast_node_t *ast_new_decl(ast_decl_type_t type, char *lval, ast_node_t *rval) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_DECL;
  node->c.decl.type = type;
  node->c.decl.lval = lval;
  node->c.decl.rval = rval;
  return node;
}

ast_node_t *ast_new_cond(ast_node_t *condition, stmt_t *body,
                         stmt_t *else_body) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_COND;
  node->c.cond.condition = condition;
  node->c.cond.body = body;
  node->c.cond.else_body = else_body;
  return node;
}

ast_node_t *ast_new_loop(stmt_t *init, ast_node_t *cond, stmt_t *end,
                         stmt_t *body) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_LOOP;
  node->c.loop.initializers = init;
  node->c.loop.condition = cond;
  node->c.loop.end = end;
  node->c.loop.body = body;
  return node;
}

ast_node_t *ast_new_constant(constant_t constant) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_CONST;
  node->c.constant = constant;
  return node;
}

ast_node_t *ast_new_symbol(char *symbol) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_SYMBOL;
  node->c.symbol = symbol;
  return node;
}

ast_node_t *ast_decl_from_assign(ast_decl_type_t type, ast_node_t *node) {
  assert(node->type == NODE_ASSIGN);
  node->type = NODE_DECL;
  node->c.decl.type = type;
  return node;
}

static quad_op_t binary_map[] = {
    [OP_ADD] = QUAD_OP_ADD, [OP_AND] = QUAD_NOOP,   [OP_DIV] = QUAD_OP_DIV,
    [OP_EQ] = QUAD_NOOP,    [OP_GTE] = QUAD_NOOP,   [OP_GT] = QUAD_NOOP,
    [OP_LTE] = QUAD_NOOP,   [OP_LT] = QUAD_NOOP,    [OP_MUL] = QUAD_OP_MUL,
    [OP_OR] = QUAD_NOOP,    [OP_SUB] = QUAD_OP_SUB,
};

quad_op_t ast_map_binary(ast_binary_op_t op) { return binary_map[op]; }

static quad_op_t unary_map[] = {
    [OP_ABS] = QUAD_OP_ABS,   [OP_CPOW] = QUAD_NOOP,    [OP_CPOWF] = QUAD_NOOP,
    [OP_CPOWL] = QUAD_NOOP,   [OP_CSIN] = QUAD_NOOP,    [OP_CSINF] = QUAD_NOOP,
    [OP_CSINL] = QUAD_NOOP,   [OP_CSQRT] = QUAD_NOOP,   [OP_CSQRTF] = QUAD_NOOP,
    [OP_CSQRTL] = QUAD_NOOP,  [OP_DECR] = QUAD_OP_DECR, [OP_EXP] = QUAD_OP_EXP,
    [OP_INCR] = QUAD_OP_INCR, [OP_LOG] = QUAD_OP_LOG,   [OP_NEG] = QUAD_OP_NEG,
    [OP_POW] = QUAD_OP_POW,   [OP_POWF] = QUAD_NOOP,    [OP_POWL] = QUAD_NOOP,
    [OP_SIN] = QUAD_OP_SIN,   [OP_SINF] = QUAD_NOOP,    [OP_SINL] = QUAD_NOOP,
    [OP_SQRT] = QUAD_OP_SQRT, [OP_SQRTF] = QUAD_NOOP,   [OP_SQRTL] = QUAD_NOOP,
};

quad_op_t ast_map_unary(ast_unary_op_t op) { return unary_map[op]; }

symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops) {
  switch (node->type) {
    case NODE_UNARY: {
      symbol_t *dest = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_t *arg = ast_gen_quad(node->c.unary.arg, table, ops);
      op_t *quad = quad_new(ast_map_unary(node->c.unary.type), dest, arg, NULL);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_BINARY: {
      symbol_t *dest = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_t *left = ast_gen_quad(node->c.binary.left, table, ops);
      symbol_t *right = ast_gen_quad(node->c.binary.right, table, ops);
      op_t *quad =
          quad_new(ast_map_binary(node->c.binary.type), dest, left, right);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_ASSIGN: {
      symbol_t *dest = symbol_lookup(table, node->c.assign.lval);
      dest->modified = true;
      symbol_t *temp = ast_gen_quad(node->c.assign.rval, table, ops);
      op_t *quad = quad_new(QUAD_OP_ASSIGN, dest, temp, NULL);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_DECL: {
      // TODO(sandhose): The symbol type should be set according to the decl type
      symbol_t *dest = symbol_add(table, SYM_DECIMAL, node->c.assign.lval, false);
      if (node->c.assign.rval) {
        dest->modified = true;
        symbol_t *temp = ast_gen_quad(node->c.assign.rval, table, ops);
        op_t *quad = quad_new(QUAD_OP_ASSIGN, dest, temp, NULL);
        quad_list_append(ops, quad);
      }
      return NULL;
    }

    case NODE_COND: {
      // TODO(sandhose): generate code for `if` control blocks
      break;
    }

    case NODE_LOOP: {
      // TODO(sandhose): generate code for loop control blocks
      break;
    }

    case NODE_CONST: {
      symbol_t *symbol = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_set_decimal(symbol, node->c.constant);
      return symbol;
    }

    case NODE_SYMBOL: {
      symbol_t *tmp = symbol_lookup(table, node->c.symbol);
      if (!tmp->modified)
        tmp->readBeforeModified = true;
      return tmp;
    }
  }

  return NULL;
}

void ast_delete(ast_node_t *node) {
  switch (node->type) {
    case NODE_UNARY:
      ast_delete(node->c.unary.arg);
      break;

    case NODE_BINARY:
      ast_delete(node->c.binary.left);
      ast_delete(node->c.binary.right);
      break;

    case NODE_ASSIGN:
      // symbol_delete(node->c.assign.lval);
      ast_delete(node->c.assign.rval);
      break;

    case NODE_DECL:
      // symbol_delete(node->c.decl.lval);
      ast_delete(node->c.decl.rval);
      break;

    case NODE_COND:
      ast_delete(node->c.cond.condition);
      // TODO(sandhose): free the statements
      break;

    case NODE_LOOP:
      ast_delete(node->c.loop.condition);
      // TODO(sandhose): free the statements
      break;

    case NODE_CONST:
      break;

    case NODE_SYMBOL:
      // symbol_delete(node->c.symbol);
      break;
  }

  free(node);
}

void indent(uint8_t n) {
  for (uint8_t i = 0; i < n; i++)
    fprintf(stderr, "  ");
}

void ast_display_i(ast_node_t *node, uint8_t i);

void stmt_display_i(stmt_t *list, uint8_t i) {
  while (list) {
    ast_display_i(list->node, i);
    list = list->next;
  }
}

void ast_display_i(ast_node_t *node, uint8_t i) {
  indent(i);

  switch (node->type) {
    case NODE_UNARY:
      fprintf(stderr, "Unary op %d\n", node->c.unary.type);
      ast_display_i(node->c.unary.arg, i + 1);
      break;

    case NODE_BINARY:
      fprintf(stderr, "Binary op %d\n", node->c.binary.type);
      ast_display_i(node->c.binary.left, i + 1);
      ast_display_i(node->c.binary.right, i + 1);
      break;

    case NODE_ASSIGN:
      fprintf(stderr, "Assign %s =\n", node->c.assign.lval);
      ast_display_i(node->c.assign.rval, i + 1);
      break;

    case NODE_DECL:
      if (node->c.decl.rval) {
        fprintf(stderr, "Declaration %d %s =\n", node->c.decl.type, node->c.decl.lval);
        ast_display_i(node->c.decl.rval, i + 1);
      } else {
        fprintf(stderr, "Declaration %d %s\n", node->c.decl.type, node->c.decl.lval);
      }
      break;

    case NODE_COND:
      fprintf(stderr, "If\n");
      ast_display_i(node->c.cond.condition, i + 1);
      indent(i);
      fprintf(stderr, "Then\n");
      stmt_display_i(node->c.cond.body, i + 1);

      if (node->c.cond.else_body) {
        indent(i);
        fprintf(stderr, "Else\n");
        stmt_display_i(node->c.cond.else_body, i + 1);
      }
      break;

    case NODE_LOOP:
      fprintf(stderr, "Loop\n");
      if (node->c.loop.initializers) {
        indent(i);
        fprintf(stderr, "Init\n");
        stmt_display_i(node->c.loop.initializers, i + 1);
      }

      if (node->c.loop.condition) {
        indent(i);
        fprintf(stderr, "Condition\n");
        ast_display_i(node->c.loop.condition, i + 1);
      }

      indent(i);
      fprintf(stderr, "Body\n");
      stmt_display_i(node->c.loop.body, i + 1);

      if (node->c.loop.end) {
        indent(i);
        fprintf(stderr, "End\n");
        stmt_display_i(node->c.loop.end, i + 1);
      }
      break;

    case NODE_CONST:
      fprintf(stderr, "Const %f\n", node->c.constant);
      break;

    case NODE_SYMBOL:
      fprintf(stderr, "Symbol '%s'\n", node->c.symbol);
      break;
  }
}

void ast_display(ast_node_t *node) { ast_display_i(node, 0); }
