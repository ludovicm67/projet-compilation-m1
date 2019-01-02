#include "acutest.h"

#define _TEST_LIST                                                             \
  _TEST_ITEM(ast, new)                                                         \
  _TEST_ITEM(quad, new)                                                        \
  _TEST_ITEM(symbol, memory)

#define _TEST_ITEM(scope, fun) void test_##scope##_##fun(void);
_TEST_LIST
#undef _TEST_ITEM

TEST_LIST = {
#define _TEST_ITEM(scope, fun) {#scope "/" #fun, test_##scope##_##fun},
    _TEST_LIST
#undef _TEST_ITEM
    {0}};
