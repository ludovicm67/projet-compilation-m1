#define TEST_NO_MAIN

#include "../optim.h"
#include "../gencode.h"
#include "../quad.h"
#include "acutest.h"
#include "output_check.h"

void test_optim_add_zero(void) {
  // init output check
  int fd[2];
  char buff[BUFF_SIZE];

  // init gencode args
  gencode_args_t args;
  args.file = NULL;
  args.lib = LIB_MPC;
  args.precision = 128;
  args.rounding = "MPC_RNDZZ";

  // create all required symbols
  symbol_t *symbol_table = NULL;
  symbol_t *var_x = symbol_add(&symbol_table, SYM_UNKNOWN, "x", true);
  symbol_t *var_y = symbol_add(&symbol_table, SYM_UNKNOWN, "y", true);
  symbol_t *var_z = symbol_add(&symbol_table, SYM_UNKNOWN, "z", true);
  symbol_t *tmp_1 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_1, 1.0);
  symbol_t *tmp_0 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_0, 0.0);

  // create required quads
  op_t *quad1 = quad_new(QUAD_OP_ADD, var_x, tmp_1, tmp_0);
  op_t *quad2 = quad_new(QUAD_OP_ADD, var_x, tmp_0, tmp_1);
  op_t *quad3 = quad_new(QUAD_OP_ADD, var_x, var_y, tmp_0);
  op_t *quad4 = quad_new(QUAD_OP_ADD, var_x, tmp_0, var_y);
  op_t *quad5 = quad_new(QUAD_OP_ADD, var_x, var_y, var_z);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));
  quad_list_concat(&list, quad_list_new(quad3));
  quad_list_concat(&list, quad_list_new(quad4));
  quad_list_concat(&list, quad_list_new(quad5));

  // run specific optimization on each quad
  optim_add_zero(quad1);
  optim_add_zero(quad2);
  optim_add_zero(quad3);
  optim_add_zero(quad4);
  optim_add_zero(quad5);

  // generate code
  args.file = __init_output_check(fd, buff);
  gencode_init(&args, symbol_table);
  gencode_assign(&args, symbol_table);
  gencode_operations(&args, list);
  gencode_clear(&args, symbol_table);
  fclose(args.file);
  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff,
                       "\n"
                       "  // declaration of all variables that we will use\n"
                       "  mpc_t T0; mpc_init2(T0, 128);\n"
                       "  mpc_t T1; mpc_init2(T1, 128);\n"
                       "  mpc_t T2; mpc_init2(T2, 128); // z\n"
                       "  mpc_t T3; mpc_init2(T3, 128); // y\n"
                       "  mpc_t T4; mpc_init2(T4, 128); // x\n"
                       "\n"
                       "  // assign values to some variables\n"
                       "  mpc_set_d(T0, 0.000000, MPC_RNDZZ);\n"
                       "  mpc_set_d(T1, 1.000000, MPC_RNDZZ);\n"
                       "\n"
                       "  // operations\n"
                       "  mpc_set(T4, T1, MPC_RNDZZ); // T4 = T1\n"
                       "  mpc_set(T4, T1, MPC_RNDZZ); // T4 = T1\n"
                       "  mpc_set(T4, T3, MPC_RNDZZ); // T4 = T3\n"
                       "  mpc_set(T4, T3, MPC_RNDZZ); // T4 = T3\n"
                       "  mpc_add(T4, T3, T2, MPC_RNDZZ); // T4 = T3 + T2\n"
                       "\n"
                       "  // free memory of all variables that we used\n"
                       "  mpc_clear(T0);\n"
                       "  mpc_clear(T1);\n"
                       "  mpc_clear(T2);\n"
                       "  mpc_clear(T3);\n"
                       "  mpc_clear(T4);\n"));
  }
  __clean_output_check(fd);

  // clean
  quad_list_delete(list);
  quad_delete(quad1);
  quad_delete(quad2);
  symbol_delete(symbol_table);
}

void test_optim_mul_one(void) {
  // init output check
  int fd[2];
  char buff[BUFF_SIZE];

  // init gencode args
  gencode_args_t args;
  args.file = NULL;
  args.lib = LIB_MPC;
  args.precision = 128;
  args.rounding = "MPC_RNDZZ";

  // create all required symbols
  symbol_t *symbol_table = NULL;
  symbol_t *var_x = symbol_add(&symbol_table, SYM_UNKNOWN, "x", true);
  symbol_t *var_y = symbol_add(&symbol_table, SYM_UNKNOWN, "y", true);
  symbol_t *var_z = symbol_add(&symbol_table, SYM_UNKNOWN, "z", true);
  symbol_t *tmp_1 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_1, 1.0);
  symbol_t *tmp_0 = symbol_add(&symbol_table, SYM_UNKNOWN, NULL, false);
  symbol_set_decimal(tmp_0, 0.0);

  // create required quads
  op_t *quad1 = quad_new(QUAD_OP_MUL, var_x, tmp_1, tmp_0);
  op_t *quad2 = quad_new(QUAD_OP_MUL, var_x, tmp_0, tmp_1);
  op_t *quad3 = quad_new(QUAD_OP_MUL, var_x, var_y, tmp_1);
  op_t *quad4 = quad_new(QUAD_OP_MUL, var_x, tmp_1, var_y);
  op_t *quad5 = quad_new(QUAD_OP_MUL, var_x, var_y, var_z);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));
  quad_list_concat(&list, quad_list_new(quad3));
  quad_list_concat(&list, quad_list_new(quad4));
  quad_list_concat(&list, quad_list_new(quad5));

  // run specific optimization on each quad
  optim_mul_one(quad1);
  optim_mul_one(quad2);
  optim_mul_one(quad3);
  optim_mul_one(quad4);
  optim_mul_one(quad5);

  // generate code
  args.file = __init_output_check(fd, buff);
  gencode_init(&args, symbol_table);
  gencode_assign(&args, symbol_table);
  gencode_operations(&args, list);
  gencode_clear(&args, symbol_table);
  fclose(args.file);
  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff,
                       "\n"
                       "  // declaration of all variables that we will use\n"
                       "  mpc_t T0; mpc_init2(T0, 128);\n"
                       "  mpc_t T1; mpc_init2(T1, 128);\n"
                       "  mpc_t T2; mpc_init2(T2, 128); // z\n"
                       "  mpc_t T3; mpc_init2(T3, 128); // y\n"
                       "  mpc_t T4; mpc_init2(T4, 128); // x\n"
                       "\n"
                       "  // assign values to some variables\n"
                       "  mpc_set_d(T0, 0.000000, MPC_RNDZZ);\n"
                       "  mpc_set_d(T1, 1.000000, MPC_RNDZZ);\n"
                       "\n"
                       "  // operations\n"
                       "  mpc_set(T4, T0, MPC_RNDZZ); // T4 = T0\n"
                       "  mpc_set(T4, T0, MPC_RNDZZ); // T4 = T0\n"
                       "  mpc_set(T4, T3, MPC_RNDZZ); // T4 = T3\n"
                       "  mpc_set(T4, T3, MPC_RNDZZ); // T4 = T3\n"
                       "  mpc_mul(T4, T3, T2, MPC_RNDZZ); // T4 = T3 * T2\n"
                       "\n"
                       "  // free memory of all variables that we used\n"
                       "  mpc_clear(T0);\n"
                       "  mpc_clear(T1);\n"
                       "  mpc_clear(T2);\n"
                       "  mpc_clear(T3);\n"
                       "  mpc_clear(T4);\n"));
  }
  __clean_output_check(fd);

  // clean
  quad_list_delete(list);
  quad_delete(quad1);
  quad_delete(quad2);
  symbol_delete(symbol_table);
}

// just here to test for SEGV for the moment
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
