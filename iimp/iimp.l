%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "iimp.tab.h"

%}

var [A-Za-z][A-Za-z0-9]*
num [1-9][0-9]*

%%

"+"             { return Pl; }
"-"             { return Mo; }
"*"             { return Mu; }
"\n"            { return Rt; }

{num}         { yylval.val = atoi(yytext); return I; }
.             { return (int) yytext[0]; }

%%
