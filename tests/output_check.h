#ifndef TEST_OUTPUT_CHECK_H
#define TEST_OUTPUT_CHECK_H

#include <stdio.h>

#define BUFF_SIZE 1024

FILE *__init_output_check(int *fd, char *buff);
void __clean_output_check(int *fd);

#endif
