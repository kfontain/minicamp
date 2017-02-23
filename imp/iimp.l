%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "iimp.tab.h"

%}

var [A-Za-z][A-Za-z0-9]*
num [1-9][0-9]*

%%

if            { return If; }
then          { return Th; }
else          { return El; }
while         { return Wh; }
do            { return Do; }
:=            { return Af; }
skip          { return Sk; }

{num}         { yylval.number = atoi(yytext); return I; }
{var}         { yylval.string = strdup(yytext); return V; }
.             { return (int) yytext[0]; }

%%
