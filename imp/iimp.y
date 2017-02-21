%{
  #include <stdio.h>
  void yyerror(char* s);
%}
%start start
%token Af Sk Se If Th El Wh Do Pl Mo Mu I V

%%
start: E C F

E: E Pl T
 | E Mo T
 | T
 ;

T: T Mu F
 | F
 ;

F: '(' E ')'
 | I
 | V
 ;

C : V Af E
  | Sk
  | '(' C ')'
  | If E Th C El C
  | Wh E Do C
  |  C Se C
  ;

%%

void yyerror(char* s){
  fprintf(stderr,"** error %s **\n ",s);
}

void yywrap(){
}

void main(){
  yyparse();
}
