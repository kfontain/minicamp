%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "imp.tab.h"

%}

var [A-Za-z][A-Za-z0-9\_\-]*
num [1-9][0-9]*|0

%%

"+"             { return Pl; }
"-"             { return Mo; }
"*"             { return Mu; }
skip            { return Sk; }
";"             { return Se; }
if              { return If; }
then            { return Th; }
else            { return El; }
while           { return Wh; }
do              { return Do; }
:=              { return Af; }

{num}           { yylval.val =   atoi(yytext); return I; }
{var}           { yylval.id  = strdup(yytext); return V; }


[ \t\n]         { /* ignore whitespace */ }
[.]             { ; }

%%
