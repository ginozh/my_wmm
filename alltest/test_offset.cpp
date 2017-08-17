/* offsetof example */
#include <stdio.h>      /* printf */
#include <stddef.h>     /* offsetof */

struct foo {
  char a;
  char b[10];
  char c;
};

int main ()
{
  printf ("offsetof(struct foo,a) is %d\n",(int)offsetof(struct foo,a));
  printf ("offsetof(struct foo,b) is %d\n",(int)offsetof(struct foo,b));
  printf ("offsetof(struct foo,c) is %d\n",(int)offsetof(struct foo,c));
  
  return 0;
}
