#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "statement.h"

#ifndef LOG_LEVEL
#define LOG_LEVEL __current_log_level
#endif

#define LEVEL_FATAL 1
#define COLOR_FATAL "\x1b[1;35;7m" // Magenta
#define LEVEL_ERROR 1
#define COLOR_ERROR "\x1b[1;31;7m" // Red
#define LEVEL_WARN 2
#define COLOR_WARN "\x1b[1;33;7m" // Yellow
#define LEVEL_INFO 3
#define COLOR_INFO "\x1b[1;32;7m" // Green
#define LEVEL_DEBUG 4
#define COLOR_DEBUG "\x1b[1;36;7m" // Cyan

#define CLEAR_LINE "\x1b[0G\x1b[0k"

// Logging macros
#define FATAL(MSG) { LOG(FATAL, MSG "\n"); exit(EXIT_FAILURE); }
#define FATALF(MSG, ...) { LOG(FATAL, MSG "\n", __VA_ARGS__); exit(EXIT_FAILURE); }
#define ERROR(MSG) LOG(ERROR, MSG "\n")
#define ERRORF(MSG, ...) LOG(ERROR, MSG "\n", __VA_ARGS__)
#define WARN(MSG) LOG(WARN, MSG "\n")
#define WARNF(MSG, ...) LOG(WARN, MSG "\n", __VA_ARGS__)
#define INFO(MSG) LOG(INFO, MSG "\n")
#define INFOF(MSG, ...) LOG(INFO, MSG "\n", __VA_ARGS__)
#define DEBUG(MSG) LOG(DEBUG, MSG "\n")
#define DEBUGF(MSG, ...) LOG(DEBUG, MSG "\n", __VA_ARGS__)
#define PERROR(MSG) LOG(ERROR, "%s: %s\n", MSG, strerror(errno))

// Double expansion technique to convert __LINE__ into string literal
#define S(x) #x
#define S_(x) S(x)

// Compile with -DNO_COLOR to disable them
#ifdef NO_COLOR
#define LEVEL_FMT(LEVEL) #LEVEL
#define LOC_FMT __FILE__ ":" S_(__LINE__)
#else
#define LEVEL_FMT(LEVEL) COLOR_##LEVEL " " #LEVEL " \x1b[0m"
#define LOC_FMT "\x1b[90m" __FILE__ ":" S_(__LINE__) "\x1b[0m"
#endif

// Internal macro to format log messages like [level] [file]:[line] [message]
#define LOG_FMT(LEVEL, ...) CLEAR_LINE LEVEL_FMT(LEVEL) "\t" LOC_FMT "\t" __VA_ARGS__
#define LOG(LEVEL, ...)                                                        \
  {                                                                            \
    if (LEVEL_##LEVEL <= LOG_LEVEL) {                                          \
      fprintf(stderr, LOG_FMT(LEVEL, __VA_ARGS__));                            \
      fflush(stderr);                                                          \
    }                                                                          \
  }

typedef struct parse_result_s {
  stmt_t* stmt;
  enum {
    MODE_MPC,
    MODE_MPFR
  } mode;
  int precision;
  char* rounding;
} parse_result_t;

extern uint8_t __current_log_level;
extern FILE *f_dst;

void log_level_incr(void);
void log_level_set(uint8_t);
void indent(uint8_t);

#endif // UTIL_H
