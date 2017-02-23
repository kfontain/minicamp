/*calc.h*/
#ifndef CALC_H
#define ENVIRON_H


struct Variable{
  char* name;
  int   value;
};

typedef Variable Var;

Var newvar(char* name, int value);
Var add(Var a, Var b);
Var sub(Var a, Var b);
Var mul(Var a, Var b);
void copy(Var src, Var dst);

#endif
