#ifndef AST_H
#define AST_H

#include "quad.h"
#include "symbol.h"

typedef double constant_t;

typedef enum ast_node_type_e {
  NODE_UNARY,
  NODE_BINARY,
  NODE_ASSIGN,
  NODE_DECL,
  NODE_CONST,
  NODE_SYMBOL,
} ast_node_type_t;

typedef enum ast_decl_type_e {
  TYPE_INT,
  TYPE_DOUBLE,
  TYPE_BOOL,
  TYPE_FLOAT,
  TYPE_COMPLEX
} ast_decl_type_t;

typedef enum ast_binary_op_e {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_LTE,
  OP_GTE,
  OP_EQ,
  OP_LT,
  OP_GT,
  OP_OR,
  OP_AND
} ast_binary_op_t;

typedef enum ast_unary_op_e {
  OP_INCR,
  OP_DECR,
  OP_NEG,
  OP_ABS,
  OP_CPOWF,
  OP_CPOWL,
  OP_CPOW,
  OP_CSINF,
  OP_CSINL,
  OP_CSIN,
  OP_CSQRTF,
  OP_CSQRTL,
  OP_CSQRT,
  OP_EXP,
  OP_LOG,
  OP_POWF,
  OP_POWL,
  OP_POW,
  OP_SINF,
  OP_SINL,
  OP_SIN,
  OP_SQRTF,
  OP_SQRTL,
  OP_SQRT,
} ast_unary_op_t;

typedef struct ast_node_s ast_node_t;

struct ast_node_s {
  ast_node_type_t type;
  union {
    struct {
      ast_unary_op_t type;
      ast_node_t *arg;
    } unary;

    struct {
      ast_binary_op_t type;
      ast_node_t *left;
      ast_node_t *right;
    } binary;

    // assign and decl have the same shape in memory. This is important for
    // transforming between them
    struct {
      char *lval;
      ast_node_t *rval;
    } assign;

    struct {
      char *lval;
      ast_node_t *rval;
      ast_decl_type_t type;
    } decl;

    constant_t constant;

    char *symbol;
  } c;
};

ast_node_t *ast_new_unary(ast_unary_op_t, ast_node_t *);
ast_node_t *ast_new_binary(ast_binary_op_t, ast_node_t *, ast_node_t *);
ast_node_t *ast_new_assign(char *, ast_node_t *);
ast_node_t *ast_new_decl(ast_decl_type_t, char *, ast_node_t *);
ast_node_t *ast_new_constant(constant_t);
ast_node_t *ast_new_symbol(char *);
ast_node_t *ast_decl_from_assign(ast_decl_type_t, ast_node_t *);
symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops);
void ast_delete(ast_node_t *);
void ast_display(ast_node_t *);

#endif
