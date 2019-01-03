FLAGS := -g
CFLAGS := $(FLAGS) -D_POSIX_SOURCE -D_C99_SOURCE -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -pedantic --std=c99
LDFLAGS := $(FLAGS)

SOURCES = \
	ast.c \
	lexer.c \
	quad.c \
	symbol.c \

BIN_OBJ = \
	main.o \
	parser.o \
	$(SOURCES:%.c=%.o)

TESTS = \
	tests/ast.c \
	tests/quad.c \
	tests/symbol.c \

TESTS_OBJ = \
	tests/main.o \
	parser.o \
	$(TESTS:%.c=%.o) \
	$(SOURCES:%.c=%.o)

DEPS = \
	$(SOURCES:%.c=%.d) \
	$(TESTS:%.c=%.d) \
	main.d \
	tests/main.d

BIN = main
TESTS_BIN = tests/main

$(BIN): $(BIN_OBJ)
$(TESTS_BIN): $(TESTS_OBJ)

.PHONY: cover
cover:
	$(MAKE) FLAGS="-fprofile-arcs -ftest-coverage -g -O0" test

.PHONY: release
release:
	$(MAKE) FLAGS="-O3"

%.d: %.c
	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

-include $(DEPS)

%.h %.o %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	mv y.tab.h $*.h

.PHONY: test
test: $(TESTS_BIN)
	$(TESTS_BIN)

.PHONY: clean
clean:
	$(RM) parser.h parser.c lexer.c $(BIN) $(BIN_OBJ) $(TESTS_BIN) $(TESTS_OBJ)

.PHONY: format
format:
	clang-format -i *.c *.h ./tests/*.c
