CFLAGS := -D_POSIX_SOURCE -D_C99_SOURCE -Wall -Wextra -Werror -pedantic --std=c99 -O3 -g
LDFLAGS := -O3 -g

SOURCES = \
	lexer.c \
	quad.c \
	symbol.c \

BIN_OBJ = \
	main.o \
	parser.o \
	$(SOURCES:%.c=%.o)

TESTS = \
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
	$(RM) parser.h parser.c $(BIN) $(BIN_OBJ) $(TESTS_BIN) $(TESTS_OBJ)

.PHONY: format
format:
	clang-format -i *.c *.h
