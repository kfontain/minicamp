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
      fprintf(stderr,"out of memory \n");
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

/* Depth first search algorithm for readAST */
void DFSread(struct ast *a) {
    if (a) { // a != NULL
        switch (a->nodetype) {
            case Pl  : printf("+ "); break;
            case Mo  : printf("- "); break;
            case Mu  : printf("* "); break;
            case Sk  : printf("Sk "); break;
            case Se  : printf("Se "); break;
            case If  : printf("IfThEl "); break;
            case Wh  : printf("Wh "); break;
            case Do  : printf("Do "); break;
            case Af  : printf("Af "); break;

            case I   : printf("%0.2f ", ((struct num*)a)->val); break;
            case V   : printf("%s ", ((struct var*)a)->id) ; break;
        }
        if (a->nodetype != V && a-> nodetype != I && a->l) {
            DFSread(a->l);
        }
        if (a->nodetype != V && a-> nodetype != I && a->m) {
            DFSread(a->m);
        }
        if (a->nodetype != V && a-> nodetype != I && a->r) {
            DFSread(a->r);
        }
    }
}

/* Depth first search algorithm for C3A compiler */

/* traduction IMP -> C3A */
BILQUAD compC3A(struct ast *a, int *ct, int *et)
{
    if (a) { // a != NULL
        char *arg1, *arg2, *dest;
        char *etiq = Idalloc();
        int op;
        switch (a->nodetype) {
            case Pl :
            case Mo :
            case Mu :
                op = a->nodetype;
                dest = Idalloc();
                BILQUAD val1 = compC3A(a->l, ct, et);
                BILQUAD val2 = compC3A(a->m, ct, et);
                arg1 = val1.fin->RES;
                arg2 = val2.fin->RES;
                sprintf(dest, "CT%d", (*ct)++);
                sprintf(etiq, "ET%d", (*et)++);
                BILQUAD res = creer_bilquad(creer_quad(etiq, op, arg1, arg2, dest));
                val1 = concatq(val1, val2);
                return concatq(val1, res);
            case I  :
                op = Afc;
                arg1 = Idalloc();
                dest = Idalloc();
                sprintf(arg1, "%0.2f", (((struct num *)a)->val));
                sprintf(dest, "CT%d", (*ct)++);
                sprintf(etiq, "ET%d", (*et)++);
                return creer_bilquad(creer_quad(etiq, op, arg1, NULL, dest));
            case V  :
                op = Sk;
                dest = (((struct var *)a)->id);
                sprintf(etiq, "ET%d", (*et)++);
                return creer_bilquad(creer_quad(etiq, op, NULL, NULL, dest));
            case Af :
                op = Af;
                BILQUAD tmp = compC3A(a->m, ct, et);
                arg1 = (((struct var *)a->l)->id);
                arg2 = tmp.fin->RES;
                sprintf(etiq, "ET%d", (*et)++);
                BILQUAD bq = creer_bilquad(creer_quad(etiq, op, arg1, arg2, NULL));
                return concatq(tmp, bq);
            case Sk :
                return creer_bilquad(creer_quad("", Sk, NULL, NULL, NULL));
            case Se :
                return(concatq(compC3A(a->l, ct, et), compC3A(a->m, ct, et)));
                break;
            case If :
                op = Jz;
                BILQUAD if1 = compC3A(a->l, ct, et);
                BILQUAD if2 = compC3A(a->m, ct, et);
                BILQUAD if3 = compC3A(a->r, ct, et);
                arg1 = if1.fin->RES;
                dest = if3.debut->ETIQ;
                if1 = concatq(if1,creer_bilquad(creer_quad("", op, arg1, NULL, dest)));
                sprintf(etiq, "ET%d", (*et)++);
                op = Jp;
                if1 = concatq(if1, if2);
                if1 = concatq(if1, creer_bilquad(creer_quad("", op, NULL, NULL, etiq)));
                if1 = concatq(if1, if3);
                return concatq(if1, creer_bilquad(creer_quad(etiq, Sk, NULL, NULL, NULL)));
            case Wh :
                op = Jz;
                BILQUAD wh1 = compC3A(a->l, ct, et);
                BILQUAD wh2 = compC3A(a->m, ct, et);
                sprintf(etiq, "ET%d", (*et)++);
                arg1 = wh1.fin->RES;
                dest = wh1.debut->ETIQ;
                wh1 = concatq(wh1,creer_bilquad(creer_quad("", op, arg1, NULL, etiq)));
                wh1 = concatq(wh1, wh2);
                op = Jp;
                wh1 = concatq(wh1,creer_bilquad(creer_quad("", op, NULL, NULL, dest)));
                return concatq(wh1, creer_bilquad(creer_quad(etiq, Sk, NULL, NULL, NULL)));




            break;
        }
    }
}

/* traduction C3A -> Y86 */
void debutY86()
{
  printf("                  .pos      0         #debut zone code\n");
  printf("INIT      :irmovl Data,     %%edx      #adresse de la zone de donnees\n");
  printf("           irmovl 256,      %%eax      #espace pile\n");
  printf("           addl   %%edx,     %%eax\n");
  printf("           rrmovl %%eax,     %%esp      #init pile\n");
  printf("           rrmovl %%eax,     %%ebp\n");
}

void finY86()
{
  printf("MUL       :nop                        #ssprog mult:M[M[%%edx]]:=X*Y\n");
  printf("           mrmovl 4(%%esp), %%eax       #A := X\n");
  printf("           mrmovl 8(%%esp), %%ebx       # B:= Y\n");
  printf("           andl   %%eax,    %%eax       # si A==0 return 0\n");
  printf("           je     END\n");
  printf("SIGN      :nop                        #si A <= 0 alors (X:= -A,Y:= -B)\n");
  printf("           jg     MULPLUS             #cas ou A > 0\n");
  printf("           irmovl 0,       %%ecx\n");
  printf("           subl   %%eax,    %%ecx\n");
  printf("           rrmovl %%ecx,    %%eax\n");
  printf("           rmmovl %%eax,    4(%%esp)    #X := -A\n");
  printf("           irmovl 0,       %%ecx\n");
  printf("           subl   %%ebx,    %%ecx\n");
  printf("           rrmovl %%ecx,    %%ebx\n");
  printf("           rmmovl %%ebx,    8(%%esp)    #Y := -B\n");
  printf("MULPLUS   :nop                        #ssprog X>0->M[M[%%edx]]:=X*Y\n");
  printf("           mrmovl 4(%%esp), %%eax       #A := X\n");
  printf("           andl   %%eax,    %%eax       # si X==0 return 0\n");
  printf("           je     END\n");
  printf("           irmovl 1,       %%esi       # A:=A-1\n");
  printf("           subl   %%esi,    %%eax\n");
  printf("           mrmovl 8(%%esp), %%ebx       # B:= Y\n");
  printf("           pushl  %%ebx                # empiler B, puis A\n");
  printf("           pushl  %%eax\n");
  printf("           call   MULPLUS             # M[%%edx]:= A * B=(X-1) * Y\n");
  printf("           popl   %%eax                # depiler A puis B\n");
  printf("           popl   %%eax\n");
  printf("           mrmovl 0(%%edx),  %%eax      # M[%%edx]:= M[%%edx] + Y\n");
  printf("           mrmovl 8(%%esp),  %%ebx\n");
  printf("           addl   %%ebx,     %%eax\n");
  printf("           rmmovl %%eax,     0(%%edx)   #end MUL(X<>0) ret(Z)\n");
  printf("           ret\n");
  printf("END       :irmovl 0,        %%eax      #end MUL(X==0) ret(Z)\n");
  printf("           rmmovl %%eax,     0(%%edx)\n");
  printf("           ret\n");
  printf("                .align    8           #debut zone donnees\n");
  printf("Data      :\n");
}

void compY86(struct ast *a)
{
  debutY86();
  printf("##### code a fournir ####\n");
  printf("#########################\n");
  finY86();
}


/* Executable */
void execute(struct ast *a)
{
    printf("\n##### Arbre syntaxe abstrait ######\n\n");
    DFSread(a);
    printf("\n\n##### Code C3A ######\n\n");
    BILQUAD bq = bilquad_vide();
    int ct = 0, et = 0;
    bq = compC3A(a, &ct, &et);
    bq = concatq(bq, creer_bilquad(creer_quad("", St, NULL, NULL, NULL)));
    ecrire_bilquad(bq);
    printf("\n##### Code y86 ######\n");
    //compY86(a);
    printf("\n");
    //treefree(a);
}

/* Suppression AST */
void treefree(struct ast *a)
{
  switch(a->nodetype) {
  /* three subtrees */
  case If:
    treefree(a->l);
    treefree(a->m);
    treefree(a->r);
    free(a);
  /* two subtrees */
  case Pl :
  case Mo :
  case Mu :
  case Af :
  case Wh :
  case Se :
    treefree(a->l);
    treefree(a->m);
    free(a);
  /* case double */
  case I  :
    free((struct num*)a);
  /* case variable */
  case V  :
    free((struct var*)a);

  /* no subtree */
  case Sk:
    free(a);
    break;

  default: break;
  }
}
