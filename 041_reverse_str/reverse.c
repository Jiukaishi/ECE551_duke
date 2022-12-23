#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char * str) {
  if(!str) return;
  size_t l=0, r=0;
  while(str[r]!='\0')
    r++;
  if(r!=0) r=r-1;
  if(r==0)
      return;
  while(l<r){
    char tmp;
    tmp =str[r];
    str[r] =str[l];
    str[l] = tmp;
    r--;
    l++;
  }
  return;
}

int main(void) {
  char str0[] = "";
  char str1[] = "123";
  char str2[] = "abcd";
  char str3[] = "Captain's log, Stardate 42523.7";
  char str4[] = "Hello, my name is Inigo Montoya.";
  char str5[] = "You can be my wingman anyday!";
  char str6[] = "Executor Selendis! Unleash the full power of your forces! There may be no tomorrow!";
  char * array[] = {str0, str1, str2, str3, str4, str5, str6};
  for (int i = 0; i < 7; i++) {
    reverse(array[i]);
    printf("%s\n", array[i]);
  }
  return EXIT_SUCCESS;
}
