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
            case Pl  : printf("Pl "); break;
            case Mo  : printf("Mo "); break;
            case Mu  : printf("Mu "); break;
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
void compC3A(struct ast *a, BILQUAD *bq, int ct, int et)
{
    if (a) { // a != NULL
        char *arg1, *arg2, *dest;
        int op;
        switch (a->nodetype) {
            case Pl :
            case Mo :
            case Mu :
                op = a->nodetype;
                if (a->l->nodetype == I) {
                    struct num *tmp = (struct num *)a->l;
                    sprintf(arg1, "%0.2f", tmp->val);
                }
                else if (a->l->nodetype == V) {
                    struct var *tmp = (struct var *)a->l;
                    sprintf(arg1, "%s", tmp->id);
                }
                else {
                    sprintf(arg1, "CT%d", ct);
                    ct++;
                }
                QUAD qd = creer_quad(NULL, op, arg1, arg2, NULL);
                BILQUAD bq2 = creer_bilquad(qd);
                *bq = concatq(*bq, bq2);
                break;
            case Af :
                if (a->l->nodetype == I) {
                    op = Afc;
                    struct num *tmp = (struct num *)a->l;
                    sprintf(arg1, "%0.2f", tmp->val);
                    QUAD qd = creer_quad(NULL, op, arg1, NULL, dest);
                    BILQUAD bq2 = creer_bilquad(qd);
                    *bq = concatq(*bq, bq2);
                }
                else if (a->l->nodetype == V) {
                    op = Af;
                    struct var *tmp = (struct var *)a->l;
                    sprintf(dest, "%s", tmp->id);
                    QUAD qd = creer_quad(NULL, op, arg1, arg2, NULL);
                    BILQUAD bq2 = creer_bilquad(qd);
                    *bq = concatq(*bq, bq2);
                }
                else {
                    op = Af;
                    sprintf(dest, "CT%d", ct);
                    ct++;
                    QUAD qd = creer_quad(NULL, op, arg1, arg2, NULL);
                    BILQUAD bq2 = creer_bilquad(qd);
                    *bq = concatq(*bq, bq2);
                }
                break;
        }
        if (a->nodetype != V && a-> nodetype != I && a->l) {
            compC3A(a->l, bq, ct, et);
        }
        if (a->nodetype != V && a-> nodetype != I && a->m) {
            compC3A(a->m, bq, ct, et);
        }
        if (a->nodetype != V && a-> nodetype != I && a->r) {
            compC3A(a->r, bq, ct, et);
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
    DFSread(a);
    printf("\n##### Code C3A ######\n");
    BILQUAD bq = bilquad_vide();
    int ct = 0, et = 0;
    compC3A(a, &bq, ct, et);
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
