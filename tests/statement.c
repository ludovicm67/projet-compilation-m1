#define TEST_NO_MAIN
#include "acutest.h"

#include "../statement.h"

void test_statement_count(void) {
  stmt_t *list = NULL;
  TEST_CHECK(stmt_count(list) == 0);

  stmt_t *first = stmt_new_block(NULL);
  stmt_concat(&list, first);
  TEST_CHECK(stmt_count(list) == 1);

  stmt_t *second = stmt_new_block(NULL);
  stmt_concat(&list, second);
  TEST_CHECK(stmt_count(list) == 2);

  TEST_CHECK(list == first);
  TEST_CHECK(list->next == second);
  TEST_CHECK(list->next->next == NULL);

  stmt_delete(list);
}

void test_statement_new_expr(void) {
  ast_node_t *expr = ast_new_symbol("x");
  stmt_t *node     = stmt_new_expr(expr);

  TEST_CHECK(node->type == STMT_EXPR);
  TEST_CHECK(node->c.expr == expr);

  stmt_delete(node);
}

void test_statement_new_block(void) {
  ast_node_t *expr = ast_new_symbol("x");
  stmt_t *inner    = stmt_new_expr(expr);
  stmt_t *node     = stmt_new_block(inner);

  TEST_CHECK(node->type == STMT_BLOCK);
  TEST_CHECK(node->c.block == inner);

  stmt_delete(node);
}

void test_statement_new_decl(void) {
  ast_node_t *two         = ast_new_constant(2.0);
  stmt_t *double_x_eq_two = stmt_new_decl(TYPE_DOUBLE, "x", two);

  TEST_CHECK(double_x_eq_two->type == STMT_DECL);
  TEST_CHECK(double_x_eq_two->c.decl.type == TYPE_DOUBLE);
  TEST_CHECK(strcmp(double_x_eq_two->c.decl.lval, "x") == 0);
  TEST_CHECK(double_x_eq_two->c.decl.rval == two);

  stmt_delete(double_x_eq_two);
}

void test_statement_new_cond(void) {
  ast_node_t *y     = ast_new_symbol("y");
  stmt_t *body      = stmt_new_expr(ast_new_assign("x", ast_new_constant(2.0)));
  stmt_t *else_body = stmt_new_expr(ast_new_assign("x", ast_new_constant(5.0)));
  stmt_t *cond      = stmt_new_cond(y, body, else_body);

  TEST_CHECK(cond->type == STMT_COND);
  TEST_CHECK(cond->c.cond.condition == y);
  TEST_CHECK(cond->c.cond.body == body);
  TEST_CHECK(cond->c.cond.else_body == else_body);

  stmt_delete(cond);
}

void test_statement_new_loop(void) {
  stmt_t *init = stmt_new_decl(TYPE_INT, "i", ast_new_constant(1));
  ast_node_t *cond =
      ast_new_binary(OP_LT, ast_new_symbol("i"), ast_new_constant(10));
  stmt_t *end  = stmt_new_expr(ast_new_unary(OP_INCR, ast_new_symbol("i")));
  stmt_t *body = stmt_new_decl(
      TYPE_INT, "y",
      ast_new_binary(OP_MUL, ast_new_symbol("i"), ast_new_symbol("y")));

  stmt_t *loop = stmt_new_loop(init, cond, end, body);

  TEST_CHECK(loop->type == STMT_LOOP);
  TEST_CHECK(loop->c.loop.initializers == init);
  TEST_CHECK(loop->c.loop.condition == cond);
  TEST_CHECK(loop->c.loop.end == end);
  TEST_CHECK(loop->c.loop.body == body);

  stmt_delete(loop);
}

void test_statement_new_break(void) {
  stmt_t *node = stmt_new_break();

  TEST_CHECK(node->type == STMT_BREAK);

  stmt_delete(node);
}

void test_statement_new_continue(void) {
  stmt_t *node = stmt_new_continue();

  TEST_CHECK(node->type == STMT_CONTINUE);

  stmt_delete(node);
}

void test_statement_new_return(void) {
  ast_node_t *retval = ast_new_symbol("x");
  stmt_t *node       = stmt_new_return(retval);

  TEST_CHECK(node->type == STMT_RETURN);
  TEST_CHECK(node->c.retval == retval);

  stmt_delete(node);
}

void test_statement_decl_from_assign(void) {
  ast_node_t *val    = ast_new_constant(1);
  ast_node_t *assign = ast_new_assign("x", val);
  stmt_t *decl       = stmt_decl_from_assign(TYPE_INT, assign);

  TEST_CHECK(decl->type == STMT_DECL);
  TEST_CHECK(decl->c.decl.type == TYPE_INT);
  TEST_CHECK(decl->c.decl.rval == val);
  TEST_CHECK(strcmp(decl->c.decl.lval, "x") == 0);

  stmt_delete(decl);
}
