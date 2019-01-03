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

symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops) {
  switch (node->type) {
  case NODE_UNARY: {
    symbol_t *dest = symbol_add(table, NULL, false, false, 0);
    symbol_t *arg = ast_gen_quad(node->c.unary.arg, table, ops);
    // TODO(sandhose): map ast_unary_op_t to quad_op_t
    op_t *quad =
        quad_new(node->c.unary.type == OP_SQRT ? QUAD_OP_SQRT : QUAD_OP_NEG,
                 dest, arg, NULL);
    quad_list_append(ops, quad);
    return dest;
  }

  case NODE_BINARY: {
    symbol_t *dest = symbol_add(table, NULL, false, false, 0);
    symbol_t *left = ast_gen_quad(node->c.binary.left, table, ops);
    symbol_t *right = ast_gen_quad(node->c.binary.right, table, ops);
    // TODO(sandhose): map ast_binary_op_t to quad_op_t
    op_t *quad =
        quad_new(node->c.binary.type == OP_ADD ? QUAD_OP_ADD : QUAD_OP_MUL,
                 dest, left, right);
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
    symbol_t *dest = symbol_add(table, node->c.assign.lval, false, false, 0);
    dest->modified = true;
    symbol_t *temp = ast_gen_quad(node->c.assign.rval, table, ops);
    op_t *quad = quad_new(QUAD_OP_ASSIGN, dest, temp, NULL);
    quad_list_append(ops, quad);
    return NULL;
  }

  case NODE_CONST: {
    return symbol_add(table, NULL, false, true, node->c.constant);
  }

  case NODE_SYMBOL: {
    symbol_t *tmp = symbol_lookup(table, node->c.symbol);
    if (!tmp->modified)
      tmp->readBeforeModified = true;
    return tmp;
  }
  }
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
    printf("  ");
}

void ast_display_i(ast_node_t *node, uint8_t i) {
  indent(i);

  switch (node->type) {
  case NODE_UNARY:
    printf("Unary op %d\n", node->c.unary.type);
    ast_display_i(node->c.unary.arg, i + 1);
    break;

  case NODE_BINARY:
    printf("Binary op %d\n", node->c.binary.type);
    ast_display_i(node->c.binary.left, i + 1);
    ast_display_i(node->c.binary.right, i + 1);
    break;

  case NODE_ASSIGN:
    printf("Assign %s =\n", node->c.assign.lval);
    ast_display_i(node->c.assign.rval, i + 1);
    break;

  case NODE_DECL:
    if (node->c.decl.rval) {
      printf("Declaration %d %s =\n", node->c.decl.type, node->c.decl.lval);
      ast_display_i(node->c.decl.rval, i + 1);
    } else {
      printf("Declaration %d %s\n", node->c.decl.type, node->c.decl.lval);
    }
    break;

  case NODE_CONST:
    printf("Const %f\n", node->c.constant);
    break;

  case NODE_SYMBOL:
    printf("Symbol '%s'\n", node->c.symbol);
    break;
  }
}

void ast_display(ast_node_t *node) { ast_display_i(node, 0); }
