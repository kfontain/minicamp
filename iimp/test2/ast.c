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
        BILQUAD last = bilquad_vide();
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
                last = concatq(val1, res);
                break;
            case I  :
                op = Afc;
                arg1 = Idalloc();
                dest = Idalloc();
                sprintf(arg1, "%0.2f", (((struct num *)a)->val));
                sprintf(dest, "CT%d", (*ct)++);
                sprintf(etiq, "ET%d", (*et)++);
                last = creer_bilquad(creer_quad(etiq, op, arg1, NULL, dest));
                break;
            case V  :
                op = Sk;
                dest = (((struct var *)a)->id);
                sprintf(etiq, "ET%d", (*et)++);
                last = creer_bilquad(creer_quad(etiq, op, NULL, NULL, dest));
                break;
            case Af :
                op = Af;
                BILQUAD tmp = compC3A(a->m, ct, et);
                arg1 = (((struct var *)a->l)->id);
                arg2 = tmp.fin->RES;
                sprintf(etiq, "ET%d", (*et)++);
                BILQUAD bq = creer_bilquad(creer_quad(etiq, op, arg1, arg2, NULL));
                last = concatq(tmp, bq);
                break;
            case Sk :
                last = creer_bilquad(creer_quad("", Sk, NULL, NULL, NULL));
                break;
            case Se :
                last = (concatq(compC3A(a->l, ct, et), compC3A(a->m, ct, et)));
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
                last = concatq(if1, creer_bilquad(creer_quad(etiq, Sk, NULL, NULL, NULL)));
                break;
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
                last = concatq(wh1, creer_bilquad(creer_quad(etiq, Sk, NULL, NULL, NULL)));
                break;
        }
        return last;
    }
}

/* traduction C3A -> Y86 */
void debutY86()
{
  printf("\n");
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

BILQUAD compY86(BILQUAD src)
{
    BILQUAD y86 = bilquad_vide();
    QUAD tmp = src.debut;
    char* s = Idalloc();
    int offset = 4;
    ENV envy = NULL;
    BILQUAD bquad;
    QUAD quad;
    while (tmp) { // != NULL
        switch (tmp->OP) {
            case Pl :
            case Mo :
                sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG1));
                quad = creer_quad(tmp->ETIQ, null, "mrmovl", s, "%%eax");
                bquad = creer_bilquad(quad);
                sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG2));
                quad = creer_quad("", null, "mrmovl", s, "%%ebx");
                bquad = concatq(bquad, creer_bilquad(quad));
                char *plmo = (tmp->OP == Pl) ? "addl":"subl";
                quad = creer_quad("", null, plmo, "%%eax", "%%ebx");
                bquad = concatq(bquad, creer_bilquad(quad));
                initenv(&envy, tmp->RES);
                affect(envy, tmp->RES, offset);
                sprintf(s, "%d(%%edx),", offset);
                offset = offset + 4;
                quad = creer_quad("", null, "rmmovl", "%%eax", s);
                bquad = concatq(bquad, creer_bilquad(quad));
                y86 = concatq(y86, bquad);
                break;
            case Mu :
                sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG1));
                quad = creer_quad(tmp->ETIQ, null, "mrmovl", s, "%%eax");
                bquad = creer_bilquad(quad);
                sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG2));
                quad = creer_quad("", null, "mrmovl", s, "%%ebx");
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "pushl", "%%ebx", NULL);
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "pushl", "%%eax", NULL);
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "call", "MUL", NULL);
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "popl", "%%eax", NULL);
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "popl", "%%ebx", NULL);
                bquad = concatq(bquad, creer_bilquad(quad));
                quad = creer_quad("", null, "mrmovl", "0(%%edx)", "%%eax");
                bquad = concatq(bquad, creer_bilquad(quad));
                sprintf(s, "%d(%%edx)", offset);
                initenv(&envy, tmp->RES);
                affect(envy, tmp->RES, offset);
                offset = offset + 4;
                quad = creer_quad("", null, "rmmovl", "%%eax,", s);
                bquad = concatq(bquad, creer_bilquad(quad));
                y86 = concatq(y86, bquad);
                break;
            case Af :
                sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG2));
                quad = creer_quad(tmp->ETIQ, null, "mrmovl", s, "%%eax");
                bquad = creer_bilquad(quad);
                if ((rech(tmp->ARG1, envy))) { // == NULL
                    sprintf(s, "%d(%%edx)", valch(envy, tmp->ARG1));
                }
                else {
                    initenv(&envy, tmp->ARG1);
                    affect(envy, tmp->ARG1, offset);
                    sprintf(s, "%d(%%edx)", offset);
                    offset = offset + 4;
                }
                quad = creer_quad("", null, "rmmovl", "%eax,", s);
                bquad = concatq(bquad, creer_bilquad(quad));
                y86 = concatq(y86, bquad);
                break;
            case Afc :
                sprintf(s, "%s,", tmp->ARG1);
                quad = creer_quad(tmp->ETIQ, null, "irmovl", s, "%%eax");
                bquad = creer_bilquad(quad);
                initenv(&envy, tmp->RES);
                affect(envy, tmp->RES, offset);
                sprintf(s, "%d(%%edx)", offset);
                offset = offset + 4;
                quad = creer_quad("", null, "rmmovl", "%%eax,", s);
                bquad = concatq(bquad, creer_bilquad(quad));
                y86 = concatq(y86, bquad);
                break;
            case Sk :
                y86 = concatq(y86, creer_bilquad(creer_quad(tmp->ETIQ, null, "nop", NULL, NULL)));
                if(tmp->RES != NULL){
                    if(rech(tmp->RES, envy) == NULL) {
                        initenv(&envy, tmp->RES);
                        affect(envy, tmp->RES, offset);
                        sprintf(s, "%d(%%edx)", offset);
                        offset = offset + 4;
                    }
                }
                break;
            case St :
                y86 = concatq(y86, creer_bilquad(creer_quad(tmp->ETIQ, null, "halt", NULL, NULL)));
                break;
            case Jz :
            	sprintf(s, "%d(%%edx),", valch(envy, tmp->ARG1));
            	quad = creer_quad(tmp->ETIQ, null, "mrmovl", s, "%%eax");
            	bquad = creer_bilquad(quad);
            	quad = creer_quad("", null, "andl", "%%eax", "%%eax");
            	bquad = concatq(bquad, creer_bilquad(quad));
            	quad = creer_quad("", null, "je", tmp->RES, NULL);
            	bquad = concatq(bquad, creer_bilquad(quad));
            	y86 = concatq(y86, bquad);
            	break;
            case Jp :
                quad = creer_quad(tmp->ETIQ, null, "jump", tmp->RES, NULL);
                y86 = concatq(y86, creer_bilquad(quad));
                break;
        }
        tmp = tmp->SUIV;
    }
    return y86;
}


/* Executable */
void execute(struct ast *a)
{
    printf("\n##### Arbre syntaxe abstrait ######\n\n");
    printf("Mp ");
    DFSread(a);
    printf("\n\n##### Code C3A ######\n\n");
    BILQUAD c3a = bilquad_vide();
    int ct = 0, et = 0;
    c3a = compC3A(a, &ct, &et);
    c3a = concatq(c3a, creer_bilquad(creer_quad("", St, NULL, NULL, NULL)));
    ecrire_bilquad(c3a);
    printf("\n##### Code y86 ######\n");
    debutY86();
    BILQUAD y86 = compY86(c3a);
    ecrire_bilquad(y86);
    finY86();
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
