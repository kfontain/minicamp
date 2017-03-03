CC= gcc

CFLAGS:= -std=c99 -Wall

all: InterIMP

InterIMP.tab.c InterIMP.tab.h: InterIMP.y ../util/environ.h
		bison -d InterIMP.y	

lex.yy.c: InterIMP.l InterIMP.tab.h ../util/environ.h
		flex InterIMP.l

InterIMP: lex.yy.c InterIMP.tab.c ../util/environ.c InterIMP.tab.h ../util/environ.h
		gcc -g -o InterIMP InterIMP.tab.c lex.yy.c ../util/environ.c

clean:
		rm InterIMP InterIMP.tab.c InterIMP.tab.h lex.yy.c
