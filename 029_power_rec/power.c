#include<stdio.h>
#include<stdlib.h>
unsigned power(unsigned x, unsigned y){
unsigned a=x, b=y;
if(b==0U)return 1; 
unsigned s = power(a,b/2);
if(b%2==1)return s*s*a;
else return s*s;
return 1;

}
