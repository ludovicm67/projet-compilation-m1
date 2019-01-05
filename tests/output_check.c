#include "output_check.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *__init_output_check(int *fd, char *buff) {
  if (pipe(fd) < 0) {
    perror("fd");
    exit(1);
  }
  memset(buff, 0, BUFF_SIZE - 1);
  return fdopen(fd[1], "a");
}

void __clean_output_check(int *fd) {
  close(fd[0]);
  close(fd[1]);
}
