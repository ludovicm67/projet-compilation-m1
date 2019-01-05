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
  NODE_DECL,
  NODE_CONST,
  NODE_SYMBOL,
  NODE_COND,
  NODE_LOOP,
  NODE_BREAK,
  NODE_CONTINUE,
  NODE_RETURN,
} ast_node_type_t;

/**
 * The different type of symbols in declarations.
 */
typedef enum ast_decl_type_e {
  TYPE_INT,
  TYPE_DOUBLE,
  TYPE_BOOL,
  TYPE_FLOAT,
  TYPE_COMPLEX
} ast_decl_type_t;

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

    // assign and decl have the same shape in memory. This is important for
    // transforming between them
    struct {
      char *lval; ///< The name of the symbol to assign.
      ast_node_t *rval; ///< The value of the assignment.
    } assign; ///< Content of a `NODE_ASSIGN`.

    struct {
      char *lval; ///< The name of the symbol to declare.
      ast_node_t *rval; ///< The value of the assignment.
      ast_decl_type_t type; ///< The type of symbol to declare.
    } decl; ///< Content of a `NODE_DECL`.

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
 * Create a new `NODE_DECL`.
 *
 * @see ast_decl_type_e
 *
 * @param[in] type the type of the declared symbol
 * @param[in] symbol the left part of the assignment
 * @param[in] value the right part of the assignment
 *
 * Example, `double x = 2`:
 * @code
 *  ast_node_t *two = ast_new_constant(2.0);
 *  ast_node_t *double_x_eq_two = ast_new_decl(TYPE_DOUBLE, "x", two);
 *
 *  assert(double_x_eq_two->type == NODE_DECL);
 *  assert(double_x_eq_two->c.decl.type == TYPE_DOUBLE);
 *  assert(strcmp(double_x_eq_two->c.decl.lval, "x") == 0);
 *  assert(double_x_eq_two->c.decl.rval == two);
 *
 *  ast_delete(double_x_eq_two);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_decl(ast_decl_type_t type, char *symbol, ast_node_t *value);

/**
 * Create a new `NODE_COND`.
 *
 * @param[in] cond the condition itself
 * @param[in] body the `then` part of the condition
 * @param[in] value the `else` part of the condition
 *
 * Example, `if (y) x = 2; else y = 5;`:
 * @code
 *  ast_node_t *y = ast_new_symbol("y");
 *  stmt_t *body = stmt_new(ast_new_assign("x", ast_new_constant(2.0)));
 *  stmt_t *else_body = stmt_new(ast_new_assign("x", ast_new_constant(5.0)));
 *  ast_node_t *cond = ast_new_cond(y, body, else_body);

 *  assert(cond->type == NODE_COND);
 *  assert(cond->c.cond.condition == y);
 *  assert(cond->c.cond.body == body);
 *  assert(cond->c.cond.else_body == else_body);

 *  ast_delete(cond);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_cond(ast_node_t *cond, stmt_t *body, stmt_t *body_else);

/**
 * Create a new `NODE_LOOP`.
 *
 * @param[in] init the initialization section of a `for` loop
 * @param[in] cond the condition part of the loop
 * @param[in] end the end part of a `for` loop
 * @param[in] body the body of the loop.
 *
 * Example, `for (int i = 1; i < 10; i++) y = i * y;`:
 * @code
 *  stmt_t *init = stmt_new(ast_new_decl(TYPE_INT, "i", ast_new_constant(1)));
 *  ast_node_t *cond =
 *      ast_new_binary(OP_LT, ast_new_symbol("i"), ast_new_constant(10));
 *  stmt_t *end = stmt_new(ast_new_unary(OP_INCR, ast_new_symbol("i")));
 *  stmt_t *body = stmt_new(ast_new_decl(
 *      TYPE_INT, "y",
 *      ast_new_binary(OP_MUL, ast_new_symbol("i"), ast_new_symbol("y"))));
 *
 *  ast_node_t *loop = ast_new_loop(init, cond, end, body);
 *
 *  assert(loop->type == NODE_LOOP);
 *  assert(loop->c.loop.initializers == init);
 *  assert(loop->c.loop.condition == cond);
 *  assert(loop->c.loop.end == end);
 *  assert(loop->c.loop.body == body);
 *
 *  ast_delete(loop);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_loop(stmt_t *init, ast_node_t *cond, stmt_t *end, stmt_t *body);

/**
 * Create a new `NODE_BREAK`.
 *
 * @code
 *  ast_node_t *node = ast_new_break();
 *
 *  assert(node->type == NODE_BREAK);
 *
 *  ast_delete(node);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_break(void);

/**
 * Create a new `NODE_CONTINUE`.
 *
 * @code
 *  ast_node_t *node = ast_new_continue();
 *
 *  assert(node->type == NODE_CONTINUE);
 *
 *  ast_delete(node);
 * @endcode
 *
 * @return The new node
 */
ast_node_t *ast_new_continue(void);

/**
 * Create a new `NODE_RETURN`.
 *
 * @param[in] retval the returned value
 *
 * Example, `return x;`:
 * @code
 *  ast_node_t *retval = ast_new_symbol("x");
 *  ast_node_t *node = ast_new_return(retval);
 *
 *  assert(node->type == NODE_RETURN);
 *  assert(node->c.retval == retval);
 *
 *  ast_delete(node);
 * @endcode
 */
ast_node_t *ast_new_return(ast_node_t *retval);

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
 * Create a `NODE_DECL` from a `NODE_ASSIGN`.
 *
 * @warning The input node is modified and should not be used after that
 *
 * @param[in] type
 * @param[in] node the `NODE_ASSIGN` to convert
 *
 * @return The converted node
 */
ast_node_t *ast_decl_from_assign(ast_decl_type_t type, ast_node_t *node);

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
