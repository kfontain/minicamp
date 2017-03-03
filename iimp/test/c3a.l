%{
#include "../util/environ.h"
#include "../util/bilquad.h"

int  OP;
char *ETIQ = NULL, *ARG1 = NULL, *ARG2 = NULL, *RES = NULL;

BILQUAD bq;
ENV env;

%}

%option noyywrap
%s Etiquette Operateur Argument1 Argument2 Destination

etiquette ET[0-9]+
chiffre [1-9][0-9]*|0
constante CTchiffre
variable  VAchiffre|[A-Za-z0-9\-\_]*

%%

. {BEGIN Etiquette;}

<Etiquette>etiquette {ETIQ = strdup(yytext);BEGIN Operateur;}

<Operateur>Pl        { OP = Pl; BEGIN Argument1;}
<Operateur>Mo        { OP = Mo; BEGIN Argument1;}
<Operateur>Mu        { OP = Mu; BEGIN Argument1;}
<Operateur>Af        { OP = Af; BEGIN Argument2;}
<Operateur>Afc       { OP = Afc; BEGIN Argument1;}
<Operateur>Sk        { OP = Sk; BEGIN Etiquette;}
<Operateur>Jp        { OP = Jp; BEGIN Destination;}
<Operateur>Jz        { OP = Jz; BEGIN Argument1;}
<Operateur>St        { OP = St; return(1);}

<Argument1>{chiffre}|{constante}|{variable}     {switch( OP ){
                                                       case 0:
                                                          strcpy(yytext, ARG1);
                                                          initenv(&env, ARG1);
                                                          break;
                                                       case 1:
                                                          strcpy(yytext, ARG1);
                                                          initenv(&env, ARG1);
                                                          break;
                                                       case 2:
                                                          strcpy(yytext, ARG1);
                                                          initenv(&env, ARG1);
                                                          break;
                                                       case 3:
                                                          strcpy(yytext, ARG1);
                                                          initenv(&env, ARG1);
                                                          break;
                                                       case 4:
                                                          strcpy(yytext, ARG1);
                                                          break;
                                                       case 7:
                                                          if (valch(env, ARG1) ==0){ BEGIN Destination;}else{ BEGIN Etiquette;}
                                                          break;
                                                   };}




<Argument2>{chiffre}|{constante}|{variable}      {switch( OP ){
                                                         case 0:
                                                            strcpy(yytext, ARG2);
                                                            break;
                                                         case 1:
                                                            strcpy(yytext, ARG2);
                                                            break;
                                                         case 2:
                                                            strcpy(yytext, ARG2);
                                                            break;
                                                         case 3:
                                                            strcpy(yytext, ARG2);
                                                            affect(env, ARG1, valch(env, ARG1));
                                                            break;
                                                   };}

<Destination>{etiquette}                {if( (OP == 6) || (OP == 7)) {rechbq(yytext, bq);}; }

<Destination>{constante}|{variable}              {affect(env, RES, eval( OP, valch(env,ARG1),valch(env, ARG2)));}
<Destination>"\n"           { QUAD q = creer_quad(ETIQ, OP, ARG1, ARG2, RES); BILQUAD bq2 = creer_bilquad(q); bq = concatq(bq,bq2); ETIQ = ARG1 = ARG2 = RES = NULL; BEGIN Etiquette;}

%%

int main()
{
bq = bilquad_vide();
env = Envalloc();
while (yylex() != 0);

printf("************Code C3A*********\n");
ecrire_bilquad(bq);
printf("******** Interpretation C3A *****\n");
ecrire_env(env);
return 0;
}
