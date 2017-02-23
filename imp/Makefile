CC= gcc

CFLAGS:= -std=c99 -Wall

all: iimp

iimp.tab.c iimp.tab.h: iimp.y
		bison -d iimp.y

lex.yy.c: iimp.l iimp.tab.h
		flex iimp.l

iimp: lex.yy.c iimp.tab.c iimp.tab.h
		gcc -o iimp iimp.tab.c lex.yy.c

clean:
		rm iimp iimp.tab.c iimp.tab.h lex.yy.c
