%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "../util/environ.h"

  void yyerror(const char* s);
  extern int yylex();
  ENV env;
%}

%start debut

%union{
  char* string;
  int val;
 }

%token<val> PL MO MU If Th El Wh Do Se SK AF 
%token<val>  I 
%token<string>  V

%type<val> C Co E T F

%left '('
%nonassoc If
%nonassoc El
%%

debut: C            { ecrire_env(env); }

C : C Se Co        { $$ = $1 ; $$ = $3; }
  | Co              { $$ = $1;}
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
  | V               { $$ = valch(env, $1); }
  ;

Co : V AF E          { initenv(&env,$1); affect(env,$1, $3); }
  | SK Se C          { $$ = $3; }
  | '(' C ')'        { $$ = $2; }
  | If E Th C El Co  { if($2){ $$ = $4;} else{ $$ = $6;}; }
  | Wh E Do Co       {do{ $$ = $4;} while($2); }
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
  yyparse();
  
  return 0;
}


