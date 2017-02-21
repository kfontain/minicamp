%{
  #include <stdio.h>
  #include <stdlib.h>

  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  void yyerror(const char* s);
%}

%token Af

%%
start: E

E: Af { printf("patate\n"); }

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
