#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "util.h"

ast_node_t *ast_alloc(ast_node_type_t type) {
  ast_node_t *node = malloc(sizeof(ast_node_t));
  node->type = type;
  return node;
}

ast_node_t *ast_new_unary(ast_unary_op_t type, ast_node_t *arg) {
  ast_node_t *node = ast_alloc(NODE_UNARY);
  node->c.unary.type = type;
  node->c.unary.arg = arg;
  return node;
}

ast_node_t *ast_new_binary(ast_binary_op_t type, ast_node_t *left,
                           ast_node_t *right) {
  ast_node_t *node = ast_alloc(NODE_BINARY);
  node->c.binary.type = type;
  node->c.binary.left = left;
  node->c.binary.right = right;
  return node;
}

ast_node_t *ast_new_assign(char *lval, ast_node_t *rval) {
  ast_node_t *node = ast_alloc(NODE_ASSIGN);
  node->c.assign.lval = strdup(lval);
  node->c.assign.rval = rval;
  return node;
}

ast_node_t *ast_new_constant(constant_t constant) {
  ast_node_t *node = ast_alloc(NODE_CONST);
  node->c.constant = constant;
  return node;
}

ast_node_t *ast_new_symbol(char *symbol) {
  ast_node_t *node = ast_alloc(NODE_SYMBOL);
  node->c.symbol = strdup(symbol);
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
    [OP_ABS] = QUAD_OP_ABS,     [OP_CPOW] = QUAD_OP_POW,
    [OP_CPOWF] = QUAD_OP_POW,   [OP_CPOWL] = QUAD_OP_POW,
    [OP_CSIN] = QUAD_OP_SIN,    [OP_CSINF] = QUAD_OP_SIN,
    [OP_CSINL] = QUAD_OP_SIN,   [OP_CSQRT] = QUAD_OP_SQRT,
    [OP_CSQRTF] = QUAD_OP_SQRT, [OP_CSQRTL] = QUAD_OP_SQRT,
    [OP_DECR] = QUAD_OP_DECR,   [OP_EXP] = QUAD_OP_EXP,
    [OP_INCR] = QUAD_OP_INCR,   [OP_LOG] = QUAD_OP_LOG,
    [OP_NEG] = QUAD_OP_NEG,     [OP_POW] = QUAD_OP_POW,
    [OP_POWF] = QUAD_OP_POW,    [OP_POWL] = QUAD_OP_POW,
    [OP_SIN] = QUAD_OP_SIN,     [OP_SINF] = QUAD_OP_SIN,
    [OP_SINL] = QUAD_OP_SIN,    [OP_SQRT] = QUAD_OP_SQRT,
    [OP_SQRTF] = QUAD_OP_SQRT,  [OP_SQRTL] = QUAD_OP_SQRT,
    [OP_LLABS] = QUAD_OP_ABS,   [OP_LABS] = QUAD_OP_ABS,
    [OP_CABSF] = QUAD_OP_ABS,   [OP_CABSL] = QUAD_OP_ABS,
    [OP_CABS] = QUAD_OP_ABS,    [OP_CEXPF] = QUAD_OP_EXP,
    [OP_CEXPL] = QUAD_OP_EXP,   [OP_CEXP] = QUAD_OP_EXP,
    [OP_EXPF] = QUAD_OP_EXP,    [OP_EXPL] = QUAD_OP_EXP,
    [OP_CLOGF] = QUAD_OP_LOG,   [OP_CLOGL] = QUAD_OP_LOG,
    [OP_CLOG] = QUAD_OP_LOG,    [OP_LOGF] = QUAD_OP_LOG,
    [OP_LOGL] = QUAD_OP_LOG,    [OP_CCOSF] = QUAD_OP_COS,
    [OP_CCOSL] = QUAD_OP_COS,   [OP_CCOS] = QUAD_OP_COS,
    [OP_COSF] = QUAD_OP_COS,    [OP_COSL] = QUAD_OP_COS,
    [OP_COS] = QUAD_OP_COS,
};

quad_op_t ast_map_unary(ast_unary_op_t op) { return unary_map[op]; }

symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops) {
  switch (node->type) {
    case NODE_UNARY: {
      symbol_t *dest = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_t *arg = ast_gen_quad(node->c.unary.arg, table, ops);
      quad_op_t op = ast_map_unary(node->c.unary.type);
      op_t *quad = quad_new(op, dest, arg, NULL);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_BINARY: {
      symbol_t *dest = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_t *left = ast_gen_quad(node->c.binary.left, table, ops);
      symbol_t *right = ast_gen_quad(node->c.binary.right, table, ops);
      quad_op_t op = ast_map_binary(node->c.binary.type);
      op_t *quad = quad_new(op, dest, left, right);
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

    case NODE_CONST: {
      symbol_t *symbol = symbol_add(table, SYM_DECIMAL, NULL, false);
      symbol_set_decimal(symbol, node->c.constant);
      DEBUGF("New symbol for constant `%f'", node->c.constant);
      return symbol;
    }

    case NODE_SYMBOL: {
      symbol_t *tmp = symbol_lookup(table, node->c.symbol);
      if (!tmp->modified)
        tmp->readBeforeModified = true;
      return tmp;
    }
  }

  FATALF("Unknown node type %d", node->type);
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
      free(node->c.assign.lval);
      ast_delete(node->c.assign.rval);
      break;

    case NODE_CONST:
      break;

    case NODE_SYMBOL:
      free(node->c.symbol);
      break;
  }

  free(node);
}

void stmt_display_i(stmt_t *list, uint8_t i);

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

    case NODE_CONST:
      fprintf(stderr, "Const %f\n", node->c.constant);
      break;

    case NODE_SYMBOL:
      fprintf(stderr, "Symbol '%s'\n", node->c.symbol);
      break;
  }
}

void ast_display(ast_node_t *node) { ast_display_i(node, 0); }
