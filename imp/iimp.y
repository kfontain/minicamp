%{
  #include <stdio.h>
  void yyerror(char* s);
%}

%%

E: E Pl T
 | E Mo T
 | T
 ;

T: T Mu F
 | F
 ;

F: ’(’ E ’)’
 | I
 | V
 ;

C: V Af E
 | Sk
 | ’(’ C ’)’
 | If E Th C El C
 | Wh E Do C
 |  C Se C
 ;

%%

void yyerror (char *s)
{
  fprintf(stderr, "***ERROR: %s ****\n",s);
  
}

void main()
{
  yyparse();
}
