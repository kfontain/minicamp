CC= gcc

CFLAGS:= -std=c99 -Wall -g

all: iimp

iimp.yacc.c: iimp.y
	bison $<

iimp.lex.c: iimp.l
	flex $<

%o: %c
	$(CC) $(CFLAGS) -o $@ $<

iimp: iimp.yacc.o iimp.lex.o
	$(CC) $(CFLAGS) $^ -o $@
	rm *.c *.o
