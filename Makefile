FLAGS := -g
CFLAGS := $(FLAGS) -D_POSIX_SOURCE -D_C99_SOURCE -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -pedantic --std=c99
LDFLAGS := $(FLAGS)

SOURCES = \
	ast.c \
	gencode.c \
	optim.c \
	quad.c \
	statement.c \
	symbol.c \
	util.c \

HEADERS = \
	$(SOURCES=.c=.h)

OBJECTS = \
	parser.o \
	lexer.o \
	$(SOURCES:%.c=%.o)

TESTS = \
	tests/ast.c \
	tests/quad.c \
	tests/statement.c \
	tests/symbol.c \
	tests/gencode.c \
	tests/optim.c \
	tests/output_check.c \

TESTS_OBJ = \
	$(TESTS:%.c=%.o)

DEPS = \
	$(SOURCES:%.c=%.d) \
	$(TESTS:%.c=%.d) \
	main.d \
	tests/main.d

BIN = main
TESTS_BIN = tests/main

$(BIN): main.o $(OBJECTS)
$(TESTS_BIN): tests/main.o $(OBJECTS) $(TESTS_OBJ)

.PHONY: cover
cover:
	$(MAKE) FLAGS="--coverage" test

.PHONY: release
release:
	$(MAKE) FLAGS="-O3"

.PHONY: doc
doc:
	doxygen

%.d: %.c
	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

-include $(DEPS)

%.h %.o %.c: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	mv y.tab.h $*.h

.PHONY: test
test: $(BIN) $(TESTS_BIN)
	$(TESTS_BIN)
	bats ./tests/e2e/test.bats

.PHONY: clean
clean:
	$(RM) parser.h parser.c lexer.c $(BIN) $(OBJECTS) $(TESTS_BIN) $(TESTS_OBJ)

.PHONY: format
format:
	clang-format -i $(SOURCES) $(TESTS) $(HEADERS) main.c tests/main.c
