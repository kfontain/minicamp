CC= gcc

CFLAGS:= -std=c99 -Wall

.phony:clean

all: imp


imp.tab.c imp.tab.h: imp.y ../util/environ.h ast.h
		bison -d imp.y	

imp.yy.c:imp.l imp.tab.h ../util/environ.h ast.h
		flex imp.l
		mv lex.yy.c $@

imp: imp.yy.c imp.tab.c ../util/environ.c ast.c imp.tab.h ../util/environ.h ast.h
		gcc -o imp imp.tab.c imp.yy.c ../util/environ.c ast.c

clean:
		rm  imp.tab.c imp.tab.h imp.yy.c imp

