%{
#include "../util/environ.h"
#include "../util/bilquad.h"
#include <ctype.h>

int  OP;
char *ETIQ = "", *ARG1 = "", *ARG2 = "", *RES = "";

BILQUAD bq;
ENV env;

%}

%option noyywrap
%s Operateur Argument1 Argument2 Destination

etiquette ET[0-9]+
chiffre [1-9][0-9]*|0
constante CTchiffre
variable  VAchiffre|[A-Z][A-Za-z0-9\-\_]*

%%

<INITIAL>{etiquette}          { ETIQ = strdup(yytext);}
<INITIAL>:                    { BEGIN Operateur;}

<Operateur>Pl        { OP = Pl;}
<Operateur>Mo        { OP = Mo;}
<Operateur>Mu        { OP = Mu;}
<Operateur>Af        { OP = Af;}
<Operateur>Afc       { OP = Afc;}
<Operateur>Sk        { OP = Sk;}
<Operateur>Jp        { OP = Jp;}
<Operateur>Jz        { OP = Jz;}
<Operateur>:         { BEGIN(Argument1);}
<Operateur>St        { OP = St; return(1);}


<Argument1>{chiffre}                            { ARG1 = strdup(yytext);}
<Argument1>{constante}|{variable}               { ARG1 = strdup(yytext); initenv(&env, ARG1);}
<Argument1>:                                    { BEGIN Argument2;}


<Argument2>{chiffre}                            { ARG2 = strdup(yytext);}
<Argument2>{constante}|{variable}               { ARG2 = strdup(yytext); initenv(&env, ARG2);} 
<Argument2>:                                    { BEGIN Destination;}

<Destination>{etiquette}                         { ;}
<Destination>{constante}|{variable}              { RES = strdup(yytext);}
<Destination>\n                                  { switch (OP){
                                                         case Pl:
                                                         case Mo:
                                                         case Mu:
                                                             initenv(&env, RES);
                                                             if(!isdigit(*ARG1)){
                                                                 if(!isdigit(*ARG2)){
                                                                      affect(env, RES, eval(OP, valch(env, ARG1), valch(env, ARG2)));
                                                                }else{
                                                                      affect(env, RES, eval(OP, valch(env, ARG1), atoi(ARG2)));}
                                                             }else{
                                                                 if(!isdigit(*ARG2)){
                                                                      affect(env, RES, eval(OP, atoi(ARG1), valch(env, ARG2)));
                                                                }else{
                                                                      affect(env, RES, eval(OP, atoi(ARG1), atoi(ARG2)));}
                                                             };
                                                             break;
                                                         case Af:
                                                             if(!isdigit(*ARG2)){ 
                                                                    affect(env, ARG1, valch(env, ARG2));
                                                             }else{ affect(env, ARG1, atoi(ARG2));};
                                                             break;
                                                         case Afc:
                                                             initenv(&env, RES);
                                                             affect(env, RES, atoi(ARG1));
                                                         default: break;
                                                      }; QUAD q = creer_quad(ETIQ, OP, ARG1, ARG2, RES); BILQUAD bq2 = creer_bilquad(q); bq = concatq(bq,bq2); ETIQ =""; ARG1 = ""; ARG2 = ""; RES = ""; BEGIN INITIAL;}


<INITIAL>\t  {;}
<Argument1>\t  {;}
<Argument2>\t  {;}
<Destination>\t  {;}

%%

int main()
{

bq = bilquad_vide();
env = Envalloc();
env = NULL;
while (yylex() != 0);

printf("\n************Code C3A*********\n");
ecrire_bilquad(bq);
printf("\n******** Interpretation C3A *****\n");
ecrire_env(env);
return 0;
}
