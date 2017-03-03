CC= gcc

CFLAGS:= -std=c99 -Wall

.phony:clean cleanex

all: InterIMP InterC3A


InterIMP.tab.c InterIMP.tab.h: InterIMP.y ../util/environ.h
		bison -d InterIMP.y	

InterIMP.yy.c: InterIMP.l InterIMP.tab.h ../util/environ.h
		flex InterIMP.l
		mv lex.yy.c $@

c3a.c: c3a.l ../util/environ.h ../util/bilquad.h
		flex c3a.l
		mv lex.yy.c $@

InterIMP: InterIMP.yy.c InterIMP.tab.c ../util/environ.c InterIMP.tab.h ../util/environ.h
		gcc -o InterIMP InterIMP.tab.c InterIMP.yy.c ../util/environ.c

InterC3A: c3a.c ../util/environ.c ../util/bilquad.c
		gcc -o InterC3A $^

clean:
		rm c3a.c InterIMP.tab.c InterIMP.tab.h InterIMP.yy.c

cleanex:
		rm InterC3A InterIMP
