CFLAGS := -D_POSIX_SOURCE -Wall -Wextra -Werror -pedantic --std=c99 -O3 -g
LDFLAGS := -O3 -g

LEXER = lexer
PARSER = parser
OBJECTS = main.o $(PARSER).o $(LEXER).o
BIN = main

$(BIN): $(OBJECTS)

%.h %.o: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	$(RM) $*.c
	mv y.tab.h $*.h

$(LEXER).o: $(PARSER).h

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(PARSER).h $(BIN)

.PHONY: format
format:
	clang-format -i *.c *.h
