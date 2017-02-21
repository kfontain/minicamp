CC= gcc

CFLAGS:= -std=c99 -Wall 

all: iimp

iimp.yacc.c:iimp.y
	bison  -o $@ $<

iimp.lex.c:iimp.l
	flex -o $@ $<

%o: %c
	$(CC) $(CFLAGS) -o $@ $<

iimp: iimp.lex.o iimp.yacc.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *o *c *~ *#
