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
  NODE_COND,
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
  OP_AND,
  OP_DIV,
  OP_EQ,
  OP_GT,
  OP_GTE,
  OP_LT,
  OP_LTE,
  OP_MUL,
  OP_OR,
  OP_SUB,
} ast_binary_op_t;

typedef enum ast_unary_op_e {
  OP_CABSF,
  OP_CABSL,
  OP_CABS,
  OP_CCOSF,
  OP_CCOSL,
  OP_CCOS,
  OP_CEXPF,
  OP_CEXPL,
  OP_CEXP,
  OP_CLOGF,
  OP_CLOGL,
  OP_CLOG,
  OP_CPOW,
  OP_CPOWF,
  OP_CPOWL,
  OP_CSIN,
  OP_CSINF,
  OP_CSINL,
  OP_CSQRT,
  OP_CSQRTF,
  OP_CSQRTL,
  OP_LLABS,
  OP_LABS,
  OP_ABS,
  OP_COSF,
  OP_COSL,
  OP_COS,
  OP_EXPF,
  OP_EXPL,
  OP_EXP,
  OP_LOGF,
  OP_LOGL,
  OP_LOG,
  OP_POW,
  OP_POWF,
  OP_POWL,
  OP_SIN,
  OP_SINF,
  OP_SINL,
  OP_SQRT,
  OP_SQRTF,
  OP_SQRTL,
  OP_NEG,
  OP_INCR,
  OP_DECR
} ast_unary_op_t;

typedef struct ast_node_s ast_node_t;

#include "statement.h"

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

    struct {
      ast_node_t *condition;
      stmt_t *body;
      stmt_t *else_body;
    } cond;

    constant_t constant;

    char *symbol;
  } c;
};

ast_node_t *ast_new_unary(ast_unary_op_t, ast_node_t *);
ast_node_t *ast_new_binary(ast_binary_op_t, ast_node_t *, ast_node_t *);
ast_node_t *ast_new_assign(char *, ast_node_t *);
ast_node_t *ast_new_decl(ast_decl_type_t, char *, ast_node_t *);
ast_node_t *ast_new_cond(ast_node_t *, stmt_t *, stmt_t *);
ast_node_t *ast_new_constant(constant_t);
ast_node_t *ast_new_symbol(char *);
ast_node_t *ast_decl_from_assign(ast_decl_type_t, ast_node_t *);
symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops);
void ast_delete(ast_node_t *);
void ast_display(ast_node_t *);

#endif
