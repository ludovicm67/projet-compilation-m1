#define TEST_NO_MAIN

#include "../optim.h"
#include "../gencode.h"
#include "../quad.h"
#include "acutest.h"

void test_optim_example(void) {
  // init gencode args
  gencode_args_t args;
  args.file = fopen("/dev/null", "a");
  args.lib = LIB_MPC;
  args.precision = 128;
  args.rounding = "MPC_RNDZZ";

  symbol_t *symbol_table = NULL;
  symbol_t *var_y = symbol_add(&symbol_table, SYM_UNKNOWN, "y", true);
  var_y->modified = true;
  symbol_t *tmp_1 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_1, 1.0);
  symbol_t *tmp_0 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_0, 0);
  symbol_t *var_z = symbol_lookup(&symbol_table, "z");
  var_z->readBeforeModified = true;
  symbol_t *var_y_1 = symbol_lookup(&symbol_table, "y");
  op_t *quad1 = quad_new(QUAD_OP_ADD, var_y, tmp_1, tmp_0);
  op_t *quad2 = quad_new(QUAD_OP_MUL, var_z, var_y_1, tmp_1);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));

  optim_arith(list);

  gencode_init(&args, symbol_table);
  gencode_assign(&args, symbol_table);
  gencode_operations(&args, list);
  gencode_clear(&args, symbol_table);

  // clean
  quad_list_delete(list);
  quad_delete(quad1);
  quad_delete(quad2);
  symbol_delete(symbol_table);
}
