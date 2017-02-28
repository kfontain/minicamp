%{
  #include <stdio.h>
  #include <stdlib.h>

  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  void yyerror(const char* s);
%}

%union {
    int val;
}

%token Pl Mo Rt
%token <val> I
%type <val> E

%left '('

%%

calc :
     | calc line
     ;

line : E Rt         { printf("%d\n", $1); }
     ;

E : I Pl I          { $$ = $1 + $3; }
  | I Mo I          { $$ = $1 - $3; }
  | I               { $$ = $1; }
  ;

%%

int main()
{
  yyin = stdin;

  do {
    yyparse();
  } while(!feof(yyin));

  return 0;
}

void yyerror(const char* s)
{
  fprintf(stderr, "Parse error : %s\n", s);
  exit(1);
}
