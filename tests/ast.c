#define TEST_NO_MAIN
#include "acutest.h"

#include "../ast.h"

void test_ast_new(void) {
  // Simple AST that represents
  //   x = 5 * sqrt(y)

  ast_node_t *five  = ast_new_constant(5);
  ast_node_t *y     = ast_new_symbol("y");
  ast_node_t *abs_y = ast_new_unary(OP_ABS, y);
  ast_node_t *op    = ast_new_binary(OP_MUL, five, abs_y);
  ast_node_t *root  = ast_new_assign("x", op);

  TEST_CHECK(five->type == NODE_CONST);
  TEST_CHECK(five->c.constant == 5);

  TEST_CHECK(y->type == NODE_SYMBOL);
  TEST_CHECK(strcmp(y->c.symbol, "y") == 0);

  TEST_CHECK(abs_y->type == NODE_UNARY);
  TEST_CHECK(abs_y->c.unary.type == OP_ABS);
  TEST_CHECK(abs_y->c.unary.arg == y);

  TEST_CHECK(op->type == NODE_BINARY);
  TEST_CHECK(op->c.binary.type == OP_MUL);
  TEST_CHECK(op->c.binary.left == five);
  TEST_CHECK(op->c.binary.right == abs_y);

  TEST_CHECK(root->type == NODE_ASSIGN);
  TEST_CHECK(strcmp(root->c.assign.lval, "x") == 0);
  TEST_CHECK(root->c.assign.rval == op);

  ast_delete(root);
}

void test_ast_new_unary(void) {
  ast_node_t *x       = ast_new_symbol("x");
  ast_node_t *minus_x = ast_new_unary(OP_NEG, x);

  TEST_CHECK(minus_x->type == NODE_UNARY);
  TEST_CHECK(minus_x->c.unary.type == OP_NEG);
  TEST_CHECK(minus_x->c.unary.arg == x);

  ast_delete(minus_x);
}

void test_ast_new_binary(void) {
  ast_node_t *two         = ast_new_constant(2.0);
  ast_node_t *x           = ast_new_symbol("x");
  ast_node_t *two_times_x = ast_new_binary(OP_MUL, two, x);

  TEST_CHECK(two_times_x->type == NODE_BINARY);
  TEST_CHECK(two_times_x->c.binary.type == OP_MUL);
  TEST_CHECK(two_times_x->c.binary.left == two);
  TEST_CHECK(two_times_x->c.binary.right == x);

  ast_delete(two_times_x);
}

void test_ast_new_assign(void) {
  ast_node_t *two             = ast_new_constant(2.0);
  ast_node_t *double_x_eq_two = ast_new_assign("x", two);

  TEST_CHECK(double_x_eq_two->type == NODE_ASSIGN);
  TEST_CHECK(strcmp(double_x_eq_two->c.assign.lval, "x") == 0);
  TEST_CHECK(double_x_eq_two->c.assign.rval == two);

  ast_delete(double_x_eq_two);
}

void test_ast_new_constant(void) {
  ast_node_t *pi = ast_new_constant(3.14);

  TEST_CHECK(pi->type == NODE_CONST);
  TEST_CHECK(pi->c.constant == 3.14);

  ast_delete(pi);
}

void test_ast_new_symbol(void) {
  ast_node_t *x = ast_new_symbol("x");

  TEST_CHECK(x->type == NODE_SYMBOL);
  TEST_CHECK(strcmp(x->c.symbol, "x") == 0);

  ast_delete(x);
}
