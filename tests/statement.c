#define TEST_NO_MAIN
#include "acutest.h"

#include "../statement.h"

void test_statement_count(void) {
  stmt_t *list = NULL;
  TEST_CHECK(stmt_count(list) == 0);

  ast_node_t *first = ast_new_constant(1);
  list = stmt_push(list, first);
  TEST_CHECK(stmt_count(list) == 1);

  ast_node_t *second = ast_new_constant(2);
  list = stmt_push(list, second);
  TEST_CHECK(stmt_count(list) == 2);

  TEST_CHECK(list->node == second);
  TEST_CHECK(list->next->node == first);
  TEST_CHECK(list->next->next == NULL);

  stmt_delete(list);
}
