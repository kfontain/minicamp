CC=gcc
CFLAGS=-g
LDFLAGS=-lfl

%.out : %.c
    $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

.PRECIOUS: %.c %.o

.PHONY: clean

clean:
    rm -f *.c *.o *.out
