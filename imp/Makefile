CC= gcc

CFLAGS:= -std=c99 -Wall -g


all: exo3 exo4 exo5

%c:%y
	bison  $<

%c:%l
	flex  -o $@ $<

%o:%c
	$(CC) $(CFLAGS) -o $@ $<


exo5: exo5.lex.o exo5.yacc.o
	$(CC) $(CFLAGS) $^ -o $@
