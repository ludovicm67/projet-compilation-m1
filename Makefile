CFLAGS := -D_POSIX_SOURCE -Wall -Wextra -Werror -pedantic --std=c99 -O3 -g
LDFLAGS := -O3 -g

LEXER = lexer
PARSER = parser
OBJECTS = main.o $(PARSER).o $(LEXER).o quad.o symbol.o
TESTS = main.o quad.o symbol.o
BIN = main

$(BIN): $(OBJECTS)

%.h %.o: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	$(RM) $*.c
	mv y.tab.h $*.h

$(LEXER).o: $(PARSER).h

tests/main: $(TESTS:%=tests/%)

.PHONY: tests
tests: tests/main
	./tests/main

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(PARSER).h $(BIN) $(TESTS:%=tests/%) tests/main

.PHONY: format
format:
	clang-format -i *.c *.h
