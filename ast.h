/**
 * @file ast.h
 *
 * Defines an abstract syntax tree structure that represents one expression.
 */

#ifndef AST_H
#define AST_H

#include "quad.h"
#include "symbol.h"

typedef double constant_t;

/**
 * The different types of nodes.
 */
typedef enum ast_node_type_e {
  NODE_UNARY,
  NODE_BINARY,
  NODE_ASSIGN,
  NODE_CONST,
  NODE_SYMBOL,
} ast_node_type_t;

/**
 * The different types of binary operations.
 *
 * It includes arithmetical operations (`*`, `/`, `+`, `-`),
 * comparaisons (`<`, `==`, `>=`…) and logical operands (`&&`, `||`)
 */
typedef enum ast_binary_op_e {
  OP_ADD, ///< `x + y`
  OP_AND, ///< `x && y`
  OP_DIV, ///< `x / y`
  OP_EQ,  ///< `x == y`
  OP_NEQ, ///< `x != y`
  OP_GT,  ///< `x > y`
  OP_GTE, ///< `x >= y`
  OP_LT,  ///< `x < y`
  OP_LTE, ///< `x <= y`
  OP_MUL, ///< `x * y`
  OP_OR,  ///< `x || y`
  OP_SUB, ///< `x - y`
} ast_binary_op_t;

/**
 * The different types of unary operations.
 *
 * Some are function calls (`abs`, `sin`…) while others are
 * unary operators (`!`, `++`…)
 */
typedef enum ast_unary_op_e {
  OP_CABSF,  ///< `cabsf(x)`
  OP_CABSL,  ///< `cabsl(x)`
  OP_CABS,   ///< `cabs(x)`
  OP_CCOSF,  ///< `ccosf(x)`
  OP_CCOSL,  ///< `ccosl(x)`
  OP_CCOS,   ///< `ccos(x)`
  OP_CEXPF,  ///< `cexpf(x)`
  OP_CEXPL,  ///< `cexpl(x)`
  OP_CEXP,   ///< `cexp(x)`
  OP_CLOGF,  ///< `clogf(x)`
  OP_CLOGL,  ///< `clogl(x)`
  OP_CLOG,   ///< `clog(x)`
  OP_CPOW,   ///< `cpow(x)`
  OP_CPOWF,  ///< `cpowf(x)`
  OP_CPOWL,  ///< `cpowl(x)`
  OP_CSIN,   ///< `csin(x)`
  OP_CSINF,  ///< `csinf(x)`
  OP_CSINL,  ///< `csinl(x)`
  OP_CSQRT,  ///< `csqrt(x)`
  OP_CSQRTF, ///< `csqrtf(x)`
  OP_CSQRTL, ///< `csqrtl(x)`
  OP_LLABS,  ///< `llabs(x)`
  OP_LABS,   ///< `labs(x)`
  OP_ABS,    ///< `abs(x)`
  OP_COSF,   ///< `cosf(x)`
  OP_COSL,   ///< `cosl(x)`
  OP_COS,    ///< `cos(x)`
  OP_EXPF,   ///< `expf(x)`
  OP_EXPL,   ///< `expl(x)`
  OP_EXP,    ///< `exp(x)`
  OP_LOGF,   ///< `logf(x)`
  OP_LOGL,   ///< `logl(x)`
  OP_LOG,    ///< `log(x)`
  OP_POW,    ///< `pow(x)`
  OP_POWF,   ///< `powf(x)`
  OP_POWL,   ///< `powl(x)`
  OP_SIN,    ///< `sin(x)`
  OP_SINF,   ///< `sinf(x)`
  OP_SINL,   ///< `sinl(x)`
  OP_SQRT,   ///< `sqrt(x)`
  OP_SQRTF,  ///< `sqrtf(x)`
  OP_SQRTL,  ///< `sqrtl(x)`
  OP_NEG,    ///< `-x`
  OP_INCR,   ///< `x++`
  OP_DECR,   ///< `x--`
} ast_unary_op_t;

typedef struct ast_node_s ast_node_t;

#include "statement.h"

/**
 * Represents a node in the abstract syntax tree.
 */
struct ast_node_s {
  ast_node_type_t type; ///< The type of node.
  union {
    struct {
      ast_unary_op_t type; ///< The type of the unary operation.
      ast_node_t *arg; ///< The node on which the operation is done.
    } unary; ///< Content of a `NODE_UNARY`.

    struct {
      ast_binary_op_t type; ///< The type of the binary operation.
      ast_node_t *left; ///< The left operand.
      ast_node_t *right; ///< The right operand.
    } binary; ///< Content of a `NODE_BINARY`.

    struct {
      char *lval; ///< The name of the symbol to assign.
      ast_node_t *rval; ///< The value of the assignment.
    } assign; ///< Content of a `NODE_ASSIGN`.

    struct {
      ast_node_t *condition; ///< The condition itself.
      stmt_t *body; ///< The `then` part of the condition.
                    ///< Can be empty.
      stmt_t *else_body; ///< The `else part of the condition.
                         ///< Can be empty.
    } cond; ///< Content of a `NODE_COND`.

    struct {
      stmt_t *initializers; ///< The initialization section of a `for` loop.
                            ///< Can be empty.
      ast_node_t *condition; ///< The condition part of the loop.
                             ///< Can be NULL for infinite loops.
      stmt_t *end; ///< The end part of a `for` loop.
                   ///< Can be empty.
      stmt_t *body; ///< The body of the loop.
                    ///< Can be empty.
    } loop; ///< Content of a `NODE_LOOP`.

    ast_node_t *retval; ///< Content of a `NODE_RETURN`.
                        ///< Can be `NULL`.

    constant_t constant; ///< Content of a `NODE_CONST`.
                         ///< For now it is a `double`.

    char *symbol; ///< Content of a `NODE_SYMBOL`.
                  ///< Contains the name of the symbol.
  } c; ///< The content of the node.
};

/**
 * Create a new `NODE_UNARY`.
 *
 * @see ast_unary_op_e
 *
 * @param[in] type the type of operation
 * @param[in] arg the argument of the operation
 *
 * Example, `-x`:
 * @code
 *  ast_node_t *x = ast_new_symbol("x");
 *  ast_node_t *minus_x = ast_new_unary(OP_NEG, x);
 *
 *  assert(minus_x->type == NODE_UNARY);
 *  assert(minus_x->c.unary.type == OP_NEG);
 *  assert(minus_x->c.unary.arg == x);
 *
 *  ast_delete(minus_x);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_unary(ast_unary_op_t type, ast_node_t *arg);

/**
 * Create a new `NODE_BINARY`.
 *
 * @see ast_binary_op_e
 *
 * @param[in] type the type of operation
 * @param[in] left the left operand
 * @param[in] right the right operand
 *
 * Example, represent `2 * x`:
 * @code
 *  ast_node_t *two = ast_new_constant(2.0);
 *  ast_node_t *x = ast_new_symbol("x");
 *  ast_node_t *two_times_x = ast_new_binary(OP_MUL, two, x);
 *
 *  assert(two_times_x->type == NODE_BINARY);
 *  assert(two_times_x->c.binary.type == OP_MUL);
 *  assert(two_times_x->c.binary.left == two);
 *  assert(two_times_x->c.binary.right == x);
 *
 *  ast_delete(two_times_x);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_binary(ast_binary_op_t type, ast_node_t *left, ast_node_t *right);

/**
 * Create a new `NODE_ASSIGN`.
 *
 * @param[in] symbol the left part of the assignment
 * @param[in] value the right part of the assignment
 *
 * Example, `x = 2`:
 * @code
 *  ast_node_t *two = ast_new_constant(2.0);
 *  ast_node_t *x_eq_two = ast_new_assign("x", two);
 *
 *  assert(x_eq_two.type == NODE_ASSIGN);
 *  assert(strcmp(x_eq_two.c.assign.lval, "x") == 0);
 *  assert(x_eq_two.c.assign.rval == two);
 *
 *  ast_delete(x_eq_two);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_assign(char *symbol, ast_node_t *value);

/**
 * Create a new `NODE_CONST`.
 *
 * @param[in] value
 *
 * Example, `3.14`:
 * @code
 *  ast_node_t *pi = ast_new_constant(3.14);
 *
 *  assert(pi->type == NODE_CONST);
 *  assert(pi->c.constant == 3.14);
 *
 *  ast_delete(pi);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_constant(constant_t value);

/**
 * Create a new `NODE_SYMBOL`.
 *
 * @param[in] symbol the symbol name
 *
 * Example, `x`:
 * @code
 *  ast_node_t *x = ast_new_symbol("x");
 *
 *  assert(x->type == NODE_SYMBOL);
 *  assert(strcmp(x->c.symbol, "x") == 0);
 *
 *  ast_delete(x);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_symbol(char *symbol);

/**
 * Generate quads from a node in the AST.
 *
 * @param[in] node the node to read
 * @param[in,out] table the table of symbols
 * @param[in,out] ops the quad list
 *
 * @return the symbol generated by this node
 */
symbol_t *ast_gen_quad(ast_node_t *node, symbol_t **table, op_list_t **ops);

/**
 * Recursively free a node from memory
 *
 * @param[in] node the node to delete
 */
void ast_delete(ast_node_t *node);

/**
 * Display a node to stderr
 *
 * @param[in] node the node to display
 */
void ast_display(ast_node_t *node);

#endif /* AST_H */
