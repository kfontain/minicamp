%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "../util/environ.h"

  extern int yylex();
  ENV env;
%}

%start debut

%union{
  char* string;
  int val;
 }

%token<val> PL MO MU If Th El Wh Do Se SK AF FIN
%token<val>  I 
%token<string>  V

%type<val> C Co E T F

%left '('

%%

debut: C            { ecrire_env(env); }

C : Co Se Co        { $$ = $1 ; $$ = $3; }
  | Co              { $$ = $1;}
  | FIN             {return 0;}
  ;
 
E : E PL T          { $$ = eval(Pl, $1, $3); }
  | E MO T          { $$ = eval(Mo, $1, $3); }
  | T               { $$ = $1; }
  ;

T : T MU F          { $$ = eval(Mu, $1, $3);  }
  | F               { $$ = $1; }
  ;

F : '(' E ')'       { $$ = $2; }
  | I               { $$ = $1; }
  | V               { $$ = valch(env, $1); printf("valeur de %s: %d\n",$1,valch(env, $1));}
  ;

Co : V AF E          { initenv(&env,$1); affect(env,$1, $3); }
  | SK               { ; }
  | '(' C ')'        { $$ = $2; }
  | If E Th C El Co  { if($2){ $$ = $4;} else{ $$ = $6;}; }
  | Wh Co Do C       { while($2){ $$ = $4;} }
  ;
%%

void yyerror(const char* s)
{
  fprintf(stderr, "Parse error : %s\n", s);
}

int main()
{
  env = Envalloc();
  env = NULL;
  while(yyparse() != 0);
  
  return 0;
}


