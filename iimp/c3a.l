%{
#include "/util/environ.h"
#include "/util/bilquad.h"

int  OP;
char *ETIQ = NULL, *ARG1 = NULL, *ARG2 = NULL, *RES = NULL;

struct cellquad *SUIV;} *QUAD;

typedef struct{
     QUAD debut;
     QUAD fin;
}BILQUAD;


%}

%s Etiquette Operateur Argument1 Argument2 Destination

etiquette ET[0-9]*
chiffre [1-9][0-9]*|0
constante CTchiffre
variable  VAchiffre|[A-Za-z0-9\-\_]*

%%

* {BEGIN Etiquette;}

<Etiquette>etiquette {BEGIN Operateur;}

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
                                                          break;
                                                       case 1:
                                                          strcpy(yytext, ARG1);
                                                          break;
                                                       case 2:
                                                          strcpy(yytext, ARG1);
                                                          break;
                                                       case 3:
                                                          strcpy(yytext, ARG1);
                                                          break;
                                                       case 7:
                                                          if ( la valeur de varible == 0){ BEGIN Destination;}else{ BEGIN Etiquette;}
                                                          break;
                                                  };}
<Argument1>{chiffre}     {if( OP == 4 ){ stocker yytext dans la structure de nom de variable Destination) }


<Argument2>{chiffre}|{constante}|{variable}      {switch( OP ){
                                                         case 0:
                                                            ARG2 = yytext;
                                                            break;
                                                         case 1:
                                                            ARG2 = yytext;
                                                            break;
                                                         case 2:
                                                            ARG2 = yytext;
                                                            break;
                                                         case 3:
                                                            ARG2 = yytext;
                                                            affecte ARG2 a la structure ou la variable de argument1 est stocke;
                                                            break;
                                                   };}

<Destination>{etiquette}                {if( (OP == 6) || (OP == 7)) rechbq(yytext, bq); }

<Destination>{constante}|{variable}              {RES = eval( OP, ARG1, ARG2); };}
<Destination>\n           { QUAD q = creer_quad(ETIQ, ARG1, ARG2, RES); BILQUAD bq2 = creer_bilquad(q); bq = concatq(bq,bq2); BEGIN Etiquette;}

%%

int main()
{
printf("************Code C3A*********\n");
ecrire_bilquad(bq);
printf("\n***********Code Y86 *********\n);
debut_y86();

fin_y86();
yylex();
}
