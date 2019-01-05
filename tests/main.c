#include "acutest.h"

#define _TEST_LIST                                                             \
  _TEST_ITEM(ast, new)                                                         \
  _TEST_ITEM(ast, decl_from_assign)                                            \
  _TEST_ITEM(quad, new)                                                        \
  _TEST_ITEM(quad, list)                                                       \
  _TEST_ITEM(statement, count)                                                 \
  _TEST_ITEM(symbol, memory)                                                   \
  _TEST_ITEM(symbol, same_name)                                                \
  _TEST_ITEM(gencode, init)                                                    \
  _TEST_ITEM(gencode, clear)                                                   \
  _TEST_ITEM(gencode, example)                                                 \
  _TEST_ITEM(optim, example)                                                   \
  _TEST_ITEM(optim, add_zero)                                                  \
  _TEST_ITEM(optim, mul_one)

#define _TEST_ITEM(scope, fun) void test_##scope##_##fun(void);
_TEST_LIST
#undef _TEST_ITEM

TEST_LIST = {
#define _TEST_ITEM(scope, fun) {#scope "/" #fun, test_##scope##_##fun},
    _TEST_LIST
#undef _TEST_ITEM
    {0}};
