%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "iimp.tab.h"

%}

var [A-Za-z]
num [1-9][0-9]*

%%

"+"             { return Pl; }
"-"             { return Mo; }
"*"             { return Mu; }
"\n"            { return Rt; }
skip            { return Sk; }
";"             { return Se; }
if              { return If; }
then            { return Th; }
else            { return El; }
while           { return Wh; }
do              { return Do; }
:=              { return Af; }

{num}           { yylval.val = atoi(yytext); return I; }
{var}           { yylval.id  = atoi(yytext); return V; }
.               { return (int) yytext[0]; }

%%
