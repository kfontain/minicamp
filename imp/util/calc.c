Var add( Var a, Var b)
{
  a.value = a.value + b.value;
  return a;
}

Var sub( Var a, Var b)
{
  a.value = a.value - b.value;
  return a;
}

Var mul( Var a, Var b)
{
  a.value = a.value * b.value;
  return a;
}

void copy( Var src, Var dst)
{
  dst.value = src.value;
}

Var newvar( char* name, int value)
{
  var tmp = malloc(sizeof(tmp));
  if (name != NULL)
    {
      int n = strlen(name);
      tmp.name = malloc(n*sizeof(char));
      tmp.name = strdup(name);
    }
  else tmp.name = NULL;

  tmp.value = value;
}
