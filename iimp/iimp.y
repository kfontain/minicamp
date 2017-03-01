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

%token Pl Mo Rt Mu
%token <val> I
%type <val> E T F

%left '('

%%

line :
     | line E Rt    { printf("%d\n", $2); }
     ;

E : I Pl T          { $$ = $1 + $3; }
  | I Mo T          { $$ = $1 - $3; }
  | T               { $$ = $1; }
  ;

T : T Mu F          { $$ = $1 * $3; }
  | F               { $$ = $1; }

F : I               { $$ = $1; }

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
