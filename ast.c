#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "util.h"

ast_node_t *ast_alloc(ast_node_type_t type) {
  ast_node_t *node = malloc(sizeof(ast_node_t));
  node->type       = type;
  return node;
}

ast_node_t *ast_new_unary(ast_unary_op_t type, ast_node_t *arg) {
  ast_node_t *node   = ast_alloc(NODE_UNARY);
  node->c.unary.type = type;
  node->c.unary.arg  = arg;
  return node;
}

ast_node_t *ast_new_binary(ast_binary_op_t type, ast_node_t *left,
                           ast_node_t *right) {
  ast_node_t *node     = ast_alloc(NODE_BINARY);
  node->c.binary.type  = type;
  node->c.binary.left  = left;
  node->c.binary.right = right;
  return node;
}

ast_node_t *ast_new_assign(char *lval, ast_node_t *rval) {
  ast_node_t *node    = ast_alloc(NODE_ASSIGN);
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
  node->c.symbol   = strdup(symbol);
  return node;
}

struct op_info {
  quad_op_t op;
  symbol_type_t input;
  symbol_type_t output;
  char *name;
};

static struct op_info binary_map[] = {
    [OP_ADD] = {QUAD_OP_ADD, SYM_DECIMAL, SYM_DECIMAL, "add"},
    [OP_SUB] = {QUAD_OP_SUB, SYM_DECIMAL, SYM_DECIMAL, "sub"},
    [OP_MUL] = {QUAD_OP_MUL, SYM_DECIMAL, SYM_DECIMAL, "mul"},
    [OP_DIV] = {QUAD_OP_DIV, SYM_DECIMAL, SYM_DECIMAL, "div"},
    [OP_EQ]  = {QUAD_NOOP, SYM_UNKNOWN, SYM_BOOLEAN, "eq"},
    [OP_NEQ] = {QUAD_NOOP, SYM_UNKNOWN, SYM_BOOLEAN, "neq"},
    [OP_AND] = {QUAD_NOOP, SYM_BOOLEAN, SYM_BOOLEAN, "and"},
    [OP_OR]  = {QUAD_NOOP, SYM_BOOLEAN, SYM_BOOLEAN, "or"},
    [OP_GTE] = {QUAD_NOOP, SYM_DECIMAL, SYM_BOOLEAN, "gte"},
    [OP_GT]  = {QUAD_NOOP, SYM_DECIMAL, SYM_BOOLEAN, "gt"},
    [OP_LTE] = {QUAD_NOOP, SYM_DECIMAL, SYM_BOOLEAN, "lte"},
    [OP_LT]  = {QUAD_NOOP, SYM_DECIMAL, SYM_BOOLEAN, "lt"},
    [OP_CPOW]   = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "cpow"},
    [OP_CPOWF]  = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "cpowf"},
    [OP_CPOWL]  = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "cpowl"},
    [OP_POW]    = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "pow"},
    [OP_POWF]   = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "powf"},
    [OP_POWL]   = {QUAD_OP_POW, SYM_DECIMAL, SYM_DECIMAL, "powl"},
};

static struct op_info unary_map[] = {
    [OP_NEG]    = {QUAD_OP_NEG, SYM_BOOLEAN, SYM_BOOLEAN, "neg"},
    [OP_ABS]    = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "abs"},
    [OP_CSIN]   = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "csin"},
    [OP_CSINF]  = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "csinf"},
    [OP_CSINL]  = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "csinl"},
    [OP_CSQRT]  = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "csqrt"},
    [OP_CSQRTF] = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "csqrtf"},
    [OP_CSQRTL] = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "csqrtl"},
    [OP_DECR]   = {QUAD_OP_DECR, SYM_DECIMAL, SYM_DECIMAL, "decr"},
    [OP_EXP]    = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "exp"},
    [OP_INCR]   = {QUAD_OP_INCR, SYM_DECIMAL, SYM_DECIMAL, "incr"},
    [OP_LOG]    = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "log"},
    [OP_SIN]    = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "sin"},
    [OP_SINF]   = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "sinf"},
    [OP_SINL]   = {QUAD_OP_SIN, SYM_DECIMAL, SYM_DECIMAL, "sinl"},
    [OP_SQRT]   = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "sqrt"},
    [OP_SQRTF]  = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "sqrtf"},
    [OP_SQRTL]  = {QUAD_OP_SQRT, SYM_DECIMAL, SYM_DECIMAL, "sqrtl"},
    [OP_LLABS]  = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "llabs"},
    [OP_LABS]   = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "labs"},
    [OP_CABSF]  = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "cabsf"},
    [OP_CABSL]  = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "cabsl"},
    [OP_CABS]   = {QUAD_OP_ABS, SYM_DECIMAL, SYM_DECIMAL, "cabs"},
    [OP_CEXPF]  = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "cexpf"},
    [OP_CEXPL]  = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "cexpl"},
    [OP_CEXP]   = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "cexp"},
    [OP_EXPF]   = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "expf"},
    [OP_EXPL]   = {QUAD_OP_EXP, SYM_DECIMAL, SYM_DECIMAL, "expl"},
    [OP_CLOGF]  = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "clogf"},
    [OP_CLOGL]  = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "clogl"},
    [OP_CLOG]   = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "clog"},
    [OP_LOGF]   = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "logf"},
    [OP_LOGL]   = {QUAD_OP_LOG, SYM_DECIMAL, SYM_DECIMAL, "logl"},
    [OP_CCOSF]  = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "ccosf"},
    [OP_CCOSL]  = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "ccosl"},
    [OP_CCOS]   = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "ccos"},
    [OP_COSF]   = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "cosf"},
    [OP_COSL]   = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "cosl"},
    [OP_COS]    = {QUAD_OP_COS, SYM_DECIMAL, SYM_DECIMAL, "cos"},
};

void adjust_operand(struct op_info info, symbol_t *operand) {
  if (operand->type == SYM_UNKNOWN)
    operand->type = info.input;

  if (info.input != SYM_UNKNOWN && operand->type != info.input) {
    FATALF("Can't use a %s symbol (%p) in a %s op",
           symbol_type_name(operand->type), (void *)operand, info.name);
  }
}

symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops) {
  switch (node->type) {
    case NODE_UNARY: {
      struct op_info info = unary_map[node->c.unary.type];
      symbol_t *arg       = ast_gen_quad(node->c.unary.arg, table, ops);
      adjust_operand(info, arg);

      symbol_t *dest = symbol_add(table, info.output, NULL, false);
      op_t *quad     = quad_new(info.op, dest, arg, NULL);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_BINARY: {
      struct op_info info = binary_map[node->c.binary.type];
      symbol_t *left      = ast_gen_quad(node->c.binary.left, table, ops);
      adjust_operand(info, left);
      symbol_t *right = ast_gen_quad(node->c.binary.right, table, ops);
      adjust_operand(info, right);

      if (info.input == SYM_UNKNOWN && left->type != right->type) {
        if (left->type == SYM_UNKNOWN)
          left->type = right->type;
        else if (right->type == SYM_UNKNOWN)
          right->type = left->type;
        else
          FATALF("Operands (%p, %p) must be the same type in %s", (void *)left,
                 (void *)right, info.name);
      }

      symbol_t *dest = symbol_add(table, info.output, NULL, false);
      op_t *quad     = quad_new(info.op, dest, left, right);
      quad_list_append(ops, quad);
      return dest;
    }

    case NODE_ASSIGN: {
      symbol_t *temp = ast_gen_quad(node->c.assign.rval, table, ops);
      symbol_t *dest = symbol_lookup(table, node->c.assign.lval);

      if (dest->type == SYM_UNKNOWN) {
        dest->type = temp->type;
      } else if (temp->type == SYM_UNKNOWN) {
        temp->type = dest->type;
      } else if (temp->type != dest->type) {
        FATALF("Can't assign %s (%p) into %s (%p)",
               symbol_type_name(temp->type), (void *)temp,
               symbol_type_name(dest->type), (void *)dest);
      }

      dest->modified = true;
      op_t *quad     = quad_new(QUAD_OP_ASSIGN, dest, temp, NULL);
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
      fprintf(stderr, "Unary op %s\n", unary_map[node->c.unary.type].name);
      ast_display_i(node->c.unary.arg, i + 1);
      break;

    case NODE_BINARY:
      fprintf(stderr, "Binary op %s\n", binary_map[node->c.binary.type].name);
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
