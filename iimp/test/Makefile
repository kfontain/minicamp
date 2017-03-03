CC= gcc

CFLAGS:= -std=c99 -Wall

all: iimp

iimp.tab.c iimp.tab.h: iimp.y ../util/environ.h
		bison -d iimp.y	

lex.yy.c: iimp.l iimp.tab.h ../util/environ.h
		flex iimp.l

iimp: lex.yy.c iimp.tab.c ../util/environ.c iimp.tab.h ../util/environ.h
		gcc -g -o iimp iimp.tab.c lex.yy.c ../util/environ.c

clean:
		rm iimp iimp.tab.c iimp.tab.h lex.yy.c
