/*
 * Declaration de l'AST pour le compilateur IMP
 */
#include "../util/environ.h"
#include "../util/bilquad.h"
#include "imp.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* interface du lexer */
extern int yylineno; /* vient du lexer */
int yyerror(const char* s);

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *m;
  struct ast *r;
};

struct num {
  int nodetype;
  double val;
};

struct var {
  int nodetype;
  char* id;
};

/* construction AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *m, struct ast *r);
struct ast *newnum(int nodetype, double val);
struct ast *newvar(int nodetype, char* id);

/*lire AST*/
void readAST(struct ast *a);

/* evaluation AST */
ENV evalAST(struct ast *a);

/*suppression et liberation de AST */
void treefree(struct ast *a);

/*Imp -> C3A*/
BILQUAD compC3A(struct ast *a, int *ct, int *et);

/*C3A -> Y86*/
BILQUAD compY86(BILQUAD src);

/*executable*/
void execute(struct ast *a);
