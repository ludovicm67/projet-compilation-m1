#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "ast.h"

ast_node_t *ast_alloc(void) {
  return malloc(sizeof(ast_node_t));
}

ast_node_t *ast_new_unary(ast_unary_op_t type, ast_node_t *arg) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_UNARY;
  node->c.unary.type = type;
  node->c.unary.arg = arg;
  return node;
}

ast_node_t *ast_new_binary(ast_binary_op_t type, ast_node_t *left, ast_node_t *right) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_BINARY;
  node->c.binary.type = type;
  node->c.binary.left = left;
  node->c.binary.right = right;
  return node;
}

ast_node_t *ast_new_assign(symbol_t *lval, ast_node_t *rval) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_ASSIGN;
  node->c.assign.lval = lval;
  node->c.assign.rval = rval;
  return node;
}

ast_node_t *ast_new_declaration(ast_decl_type_t type, symbol_t *lval, ast_node_t *rval) {
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

ast_node_t *ast_new_symbol(symbol_t *symbol) {
  ast_node_t *node = ast_alloc();
  node->type = NODE_SYMBOL;
  node->c.symbol = symbol;
  return node;
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
      printf("Declaration %d %s =\n", node->c.decl.type, node->c.decl.lval);
      ast_display_i(node->c.decl.rval, i + 1);
      break;

    case NODE_CONST:
      printf("Const %f\n", node->c.constant);
      break;

    case NODE_SYMBOL:
      printf("Symbol '%s'\n", node->c.symbol);
      break;
  }
}

void ast_display(ast_node_t *node) {
  ast_display_i(node, 0);
}
