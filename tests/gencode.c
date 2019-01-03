#define TEST_NO_MAIN

#include "../gencode.h"
#include "../quad.h"
#include "acutest.h"
#include <unistd.h>

#define BUFF_SIZE 1024

FILE *__init_output_check(int *fd, char *buff) {
  if (pipe(fd) < 0) {
    perror("fd");
    exit(EXIT_FAILURE);
  }
  memset(buff, 0, BUFF_SIZE - 1);
  return fdopen(fd[1], "a");
}

void __clean_output_check(int *fd) {
  close(fd[0]);
  close(fd[1]);
}

void test_gencode_init(void) {
  // init output check
  int fd[2];
  char buff[BUFF_SIZE];
  FILE *f = NULL;

  // init symbols
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, true, 1);
  symbol_add(&symbol_table, "a", true, true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");

  // check init
  f = __init_output_check(fd, buff);
  gencode_init(f, symbol_table, 128);

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(
        !strcmp(buff, "\n"
                      "  // declaration of all MPC variables that we will use\n"
                      "  mpc_t T0; mpc_init2(T0, 128); // x\n"
                      "  mpc_t T1; mpc_init2(T1, 128); // a\n"
                      "  mpc_t T2; mpc_init2(T2, 128); // a\n"));
  }
  __clean_output_check(fd);

  symbol_delete(symbol_table);
}

void test_gencode_clear(void) {
  // init output check
  int fd[2];
  char buff[BUFF_SIZE];
  FILE *f = NULL;

  // init symbols
  symbol_t *symbol_table = NULL;
  symbol_add(&symbol_table, "a", true, true, 1);
  symbol_add(&symbol_table, "a", true, true, 2);
  symbol_lookup(&symbol_table, "x");
  symbol_lookup(&symbol_table, "x");

  // check init
  f = __init_output_check(fd, buff);
  gencode_init(f, symbol_table, 128);

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(
        !strcmp(buff, "\n"
                      "  // declaration of all MPC variables that we will use\n"
                      "  mpc_t T0; mpc_init2(T0, 128); // x\n"
                      "  mpc_t T1; mpc_init2(T1, 128); // a\n"
                      "  mpc_t T2; mpc_init2(T2, 128); // a\n"));
  }
  __clean_output_check(fd);

  // check clear
  f = __init_output_check(fd, buff);
  gencode_clear(f, symbol_table, "MPC_RNDZZ");

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff,
                       "\n"
                       "  // free memory of all MPC variables that we used\n"
                       "  mpc_clear(T0);\n"
                       "  mpc_clear(T1);\n"
                       "  mpc_clear(T2);\n"));
  }
  __clean_output_check(fd);

  symbol_delete(symbol_table);
}

void test_gencode_example(void) {
  symbol_t *symbol_table = NULL;
  TEST_CHECK(symbol_table == NULL);

  symbol_t *var_y = symbol_add(&symbol_table, "y", false, false, 0);
  TEST_CHECK(var_y != NULL);
  var_y->modified = true;
  symbol_t *tmp_1 = symbol_add(&symbol_table, NULL, false, true, 1);
  TEST_CHECK(tmp_1 != NULL);
  symbol_t *var_x = symbol_lookup(&symbol_table, "x");
  TEST_CHECK(var_x != NULL);
  symbol_t *var_z = symbol_lookup(&symbol_table, "z");
  TEST_CHECK(var_z != NULL);
  symbol_t *var_y_1 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_1 != NULL);
  TEST_CHECK(var_y == var_y_1);
  symbol_t *var_y_2 = symbol_lookup(&symbol_table, "y");
  TEST_CHECK(var_y_2 != NULL);
  TEST_CHECK(var_y == var_y_2);

  TEST_CHECK(symbol_table != NULL);
  op_t *quad1 = quad_new(QUAD_OP_ADD, var_y, tmp_1, var_x);
  op_t *quad2 = quad_new(QUAD_OP_MUL, var_z, var_y_1, var_y_2);
  op_list_t *list = quad_list_new(quad1);
  quad_list_concat(&list, quad_list_new(quad2));

  // check length
  TEST_CHECK(list != NULL);
  TEST_CHECK(list->next != NULL);
  TEST_CHECK(list->next->next == NULL);

  // check order
  TEST_CHECK(list->quad == quad1);
  TEST_CHECK(list->next->quad == quad2);

  // init output check
  int fd[2];
  char buff[BUFF_SIZE];
  FILE *f = NULL;

  // check init
  f = __init_output_check(fd, buff);
  gencode_init(f, symbol_table, 128);

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(
        !strcmp(buff, "\n"
                      "  // declaration of all MPC variables that we will use\n"
                      "  mpc_t T0; mpc_init2(T0, 128); // z\n"
                      "  mpc_t T1; mpc_init2(T1, 128); // x\n"
                      "  mpc_t T2; mpc_init2(T2, 128);\n"
                      "  mpc_t T3; mpc_init2(T3, 128); // y\n"));
  }
  __clean_output_check(fd);

  // check assign
  f = __init_output_check(fd, buff);
  gencode_assign(f, symbol_table, "MPC_RNDZZ");

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff, "\n"
                             "  // assign values to some variables\n"
                             "  mpc_set_si(T0, z, MPC_RNDZZ); // z\n"
                             "  mpc_set_si(T1, x, MPC_RNDZZ); // x\n"
                             "  mpc_set_si(T2, 1.000000, MPC_RNDZZ);\n"));
  }
  __clean_output_check(fd);

  // check operations
  f = __init_output_check(fd, buff);
  gencode_operations(f, list, "MPC_RNDZZ");

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff,
                       "\n"
                       "  // operations\n"
                       "  mpc_add(T3, T2, T1, MPC_RNDZZ); // T3 = T2 + T1\n"
                       "  mpc_mul(T0, T3, T3, MPC_RNDZZ); // T0 = T3 * T3\n"));
  }
  __clean_output_check(fd);

  // check clean
  f = __init_output_check(fd, buff);
  gencode_clear(f, symbol_table, "MPC_RNDZZ");

  fclose(f);

  while (read(fd[0], buff, BUFF_SIZE) > 0) {
    TEST_CHECK(!strcmp(buff,
                       "  y = mpc_get_ldc(T3, MPC_RNDZZ);\n"
                       "\n"
                       "  // free memory of all MPC variables that we used\n"
                       "  mpc_clear(T0);\n"
                       "  mpc_clear(T1);\n"
                       "  mpc_clear(T2);\n"
                       "  mpc_clear(T3);\n"));
  }
  __clean_output_check(fd);

  // clean
  quad_list_delete(list);
  quad_delete(quad1);
  quad_delete(quad2);
  symbol_delete(symbol_table);
}
