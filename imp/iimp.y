%{
  #include <stdio.h>
  #include <stdlib.h>
  

  #define YYSTYPE Var
  
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  void yyerror(const char* s);
%}


%union { Var var}

%token<var> I V
%token If Th El Wh Do Af Sk

%left '('

%type<var> C E F T 

%start start

%%

start : C
      ;

E : E '+' T           { $$ = $1 + $3; }
  | E '-' T           { $$ = $1 - $3; }
  | T                 { $$ = $1; }
  ;

T : T '*' F           { $$ = $1 * $3; }
  | F                 { $$ = $1; }
  ;

F : '(' E ')'         { $$ = $2; }
  | I                 { $$ = $1; }
  | V                 { $$ = $1; }
  ;

C : V Af E            { $1 = $3; }
  | Sk                { ; }
  | '(' C ')'         { $$ = $2; }
  | If E Th C El C    { if($2) $$ = $4; else $$ = $6; }
  | Wh E Do C         { while($2) $$ = $4; }
  | C ';' C           { $$ = $1; $$ = $3; }
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
