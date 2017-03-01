%{
  #include <stdio.h>
  #include <stdlib.h>

  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  void yyerror(const char* s);

  int sym[26];
%}

%union {
    int val;
    int id;
}

%token Pl Mo Rt Mu If Th El Wh Do Se Sk Af
%token <val> I
%token <id>  V
%type  <val> E T F C

%left '('

%%

line :
     | line E Rt    { printf("%d\n", $2); }
     | line C Rt    { ; }
     ;

E : I Pl T          { $$ = $1 + $3; }
  | I Mo T          { $$ = $1 - $3; }
  | T               { $$ = $1; }
  ;

T : T Mu F          { $$ = $1 * $3; }
  | F               { $$ = $1; }
  ;

F : '(' E ')'       { $$ = $2; }
  | I               { $$ = $1; }
  | V               { $$ = sym[$1]; }
  ;

C : V Af E          { sym[$1] = $3; }
  | Sk              { ; }
  | '(' C ')'       { $$ = $2; }
  | If E Th C El C  { if($2) $$ = $4; else $$ = $6; }
  | Wh E Do C       { while($2) $$ = $4; }
  | C Se C          { $$ = $1; $$ = $3; }
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
