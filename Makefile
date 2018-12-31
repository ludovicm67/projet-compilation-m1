CFLAGS := -D_POSIX_SOURCE -Wall -Wextra -Werror -pedantic --std=c99 -O3 -g
LDFLAGS := -O3 -g

main: main.o parser.o lexer.o

%.c %.h: %.y
	$(YACC) $(YFLAGS) -d $<
	mv y.tab.c $*.c
	mv y.tab.h $*.h

lexer.o: parser.h

.PHONY: clean
clean:
	$(RM) *.o parser.h main

.PHONY: format
format:
	clang-format -i *.c *.h
