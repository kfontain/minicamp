#include "ast.h"

/* Construction AST */
struct ast* newast(int nodetype, struct ast* l, struct ast* m, struct ast* r)
{
  struct ast* a = malloc(sizeof(struct ast));
  if(!a) {
    fprintf(stderr,"out of space");
      exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->m = m;
  a->r = r;
  return a;
}

struct ast* newnum(int nodetype, double val)
{
  struct num *n = malloc(sizeof(struct num));
  if(!n)
    {
      fprintf(stderr,"out of space");
      exit(0);
    }
  n->nodetype = nodetype;
  n->val = val;
  return (struct ast*)n;
}

struct ast* newvar(int nodetype, char* id)
{
  struct var *v = malloc(sizeof(struct var));
   if(!v)
    {
      fprintf(stderr,"out of space");
      exit(0);
    }
   v->nodetype = nodetype;
   v->id = strdup(id);
   return (struct ast*)v;
}

/* lecture AST */
void readAST(struct ast *a)
{
  //Todo
}

/* Evaluation AST */
/*ENV
evalAST(struct ast *a)
{
  ENV env = Envalloc();
  double tmp;
  switch(a->nodetype) {
  case Pl : tmp = eval(Pl, evalAST(a->l), evalAST(a->m)); break;
  case Mo : tmp = eval(Mo, evalAST(a->l), evalAST(a->m)); break;
  case Mu : tmp = eval(Mu, evalAST(a->l), evalAST(a->m)); break;
  case Af : affect(env, a->l->id, a->m->val); break;
  case Sk : break;
  case Se : evalAST(a->l); evalAST(a->m); break;
  case If : if (a->l->val == 0){evalAST(a->r);} else{ evalAST(a->m);}break;
  case Th : evalAST(a->l); break;
  case El : evalAST(a->l); break;
  case Wh : if(a->l->val != 0) evalAST(a->m); break;
  case Do : evalAST(a->l); break;
  case V  : initenv(env, a->id); break;
  case I  : break;
  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return env;
  }*/

/* Suppression AST */
treefree(struct ast *a)
{
  switch(a->nodetype) {
    /* three subtrees */
  case 175:
    treefree(a->l);
    treefree(a->m);
    treefree(a->r);
    /* two subtrees */
  case Pl :
  case Mo :
  case Mu :
  case Af :
  case 184 :
  case 191 :
    treefree(a->l);
    treefree(a->m);
    /* one subtree */
  case 188 :
  case 177 :
    treefree(a->l);
        /* no subtree */
  case Sk:
    free(a);
    break;
  default: break;
  }
}

/* traduction IMP -> C3A */
void compC3A(struct ast *a)
{
  //Todo
}

/* traduction C3A -> Y86 */
void compY86(struct ast *a)
{
  //Todo
}


/* Executable */
void execute(struct ast *a)
{
readAST(a);
printf("\n");
//evalAST(a);
//printf("\n");
compC3A(a);
printf("\n");
compY86(a);
printf("\n");
treefree(a);
}
