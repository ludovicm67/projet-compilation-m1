#define TEST_NO_MAIN
#include "acutest.h"

#include "../ast.h"

void test_ast_new(void) {
  // Simple AST that represents
  //   x = 5 * sqrt(y)

  ast_node_t *five = ast_new_constant(5);
  ast_node_t *y = ast_new_symbol("y");
  ast_node_t *abs_y = ast_new_unary(OP_ABS, y);
  ast_node_t *op = ast_new_binary(OP_MUL, five, abs_y);
  ast_node_t *root = ast_new_assign("x", op);

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

void test_ast_declaration_from_assign() {
  ast_node_t *val = ast_new_constant(1);
  ast_node_t *assign = ast_new_assign("x", val);
  ast_node_t *decl = ast_declaration_from_assign(assign, TYPE_INT);

  TEST_CHECK(decl->type == NODE_DECL);
  TEST_CHECK(decl->c.decl.type == TYPE_INT);
  TEST_CHECK(decl->c.decl.rval == val);
  TEST_CHECK(strcmp(decl->c.decl.lval, "x") == 0);
}
