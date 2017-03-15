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

%token<a> Pl Mo Mu If Th El Wh Do Se Sk Af
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
  | Co                {  }
  ;

E : E Pl T            { $$ = newast(Pl, $1, $3, NULL); }
  | E Mo T            { $$ = newast(Mo, $1, $3, NULL); }
  | T                 { }
  ;

T : T Mu F            { $$ = newast(Mu, $1, $3, NULL); }
  | F                 { ; }
  ;

F : '(' E ')'         { $$ = $2; }
  | I                 { $$ = newnum(I, $1); }
  | V                 { $$ = newvar(V, $1); }
  ;

Co : V Af E           { struct ast* var = newvar(V, $1); $$ = newast(Af, var, $3, NULL); }
   | Sk Se C          { $$ = newast(Sk, NULL, NULL, NULL); }
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
  yyparse();

  return 0;
}
