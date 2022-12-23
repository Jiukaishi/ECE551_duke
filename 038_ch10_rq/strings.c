#include <stdlib.h>
#include<stdio.h>
/* Draw each variable representation. 
   Which are mutable or immutable? 
   Is each one a string? 
*/
int main(void) {
  char * s1 = "Hello";
  const char * s2 = "Hello";
  char s3[] = "Hello";
  char s4[] = {'H','e','l','l','o'};
  char s5[] = {'H','e','l','l','o','\0'};
  int s=5;int *p=&s ;
  if(p==&p)
      printf("s");

  return EXIT_SUCCESS;
}
