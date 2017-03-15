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

void DFSread(struct ast *a) {
    if (a) {
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

/* lecture AST */
void readAST(struct ast *a)
{
    printf("\n");
    DFSread(a);
    printf("\n");
}

/* Suppression AST */
void treefree(struct ast *a)
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
  /* printf("Code C3A :\n");
    printf("----------\n");

    while (a->l != NULL) {
        switch (a->nodetype) {
            case Pl  : printf("nodetype : Pl ");
            case Mo  : printf("nodetype : Mo ");
            case Mu  : printf("nodetype : Mu ");
            case Af  : printf("nodetype : Af ");
            default : break;
        }
        printf("%s ", ((struct var*)a->l)->id);
        printf("%s ", ((struct var*)a->m)->id);
        printf("\n");
        a = a->l;
	}*/
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
  printf("ET1       :nop                        #trad Sk                         X2\n");
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
    readAST(a);
    printf("\n########## Code C3A ########\n");
    compC3A(a);
    printf("\n########### Code y86 #######\n");
    //compY86(a);
    printf("\n");
    treefree(a);
}
