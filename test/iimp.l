%option noyywrap

%{

#include <stdio.h>
#include "iimp.tab.h"

%}

%%

:=  { return Af; }

%%
