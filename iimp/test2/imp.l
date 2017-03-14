%option noyywrap

%{

#include <stdio.h>
#include <string.h>
#include "imp.tab.h"

%}

var [A-Za-z][A-Za-z0-9\_\-]*
num [1-9][0-9]*|0

%%

<<EOF>>         { return FIN; }
\+              { return PL; }
\-              { return MO; }
\*              { return MU; }
skip            { return SK; }
\;              { return Se; }
if              { return If; }
then            { return Th; }
else            { return El; }
while           { return Wh; }
do              { return Do; }
:=              { return AF; }

{num}           { yylval.val =   atoi(yytext); return I; }
{var}           { yylval.id  = strdup(yytext); return V; }

[.\n ]          { ; }

%%
