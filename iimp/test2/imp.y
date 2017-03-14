%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "../util/environ.h"
  #include "ast.h"

  int yyerror(const char* s);
  extern int yylex();
%}

%union {
  struct ast *a;
  double val;
  char* id;
}

%token<a> PL MO MU If Th El Wh Do Se SK AF FIN
%token<val> I
%token<id>  V

%type<a> C Co E T F

%left '('
%nonassoc If
%nonassoc El

%start debut

%%

debut : C             { execute($1); }

C : C Se Co           { $$ = newast(Se, $1, $3, NULL); }
  | Co                {;}
  | FIN               {return 0;}
  ;

E : E PL T            { $$ = newast(Pl, $1, $3, NULL); }
  | E MO T            { $$ = newast(Mo, $1, $3, NULL); }
  | T                 { }
  ;

T : T MU F            { $$ = newast(Mu, $1, $3, NULL); }
  | F                 { ; }
  ;

F : '(' E ')'         { $$ = $2; }
  | I                 { $$ = newnum(I, $1); }
  | V                 { $$ = newvar(V, $1); }
  ;

Co : V AF E           { struct ast* var = newvar(V,$1); $$ = newast(Af, var, $3, NULL); }
   | SK C             { $$ = newast(SK, NULL, NULL, NULL); }
   | '(' C ')'        { $$ = $2; }
   | If E Th C El Co  { $$ = newast(If, $2, $3, $5); }
   | Wh E Do Co       { }
   ;

%%

int yyerror(const char* s)
{
  fprintf(stderr, "Parse error : %s\n", s);
  exit(0);
}

int main()
{
  while(yyparse() != 0);

  return 0;
}
