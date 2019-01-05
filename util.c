#include "util.h"

uint8_t __current_log_level = 0;

void log_level_incr(void) { __current_log_level++; }

void log_level_set(uint8_t level) { __current_log_level = level; }

void indent(uint8_t n) {
  for (uint8_t i = 0; i < n; i++)
    fprintf(stderr, "  ");
}
