#include<limits.h>
#include<stdio.h>
#include<stdlib.h>
unsigned power(unsigned, unsigned);

void check(unsigned a, unsigned b, unsigned truth){
    if(power(a,b)!=truth){
        printf("found bug: %d, %d",a,b);
        exit(EXIT_FAILURE);
    }
}
int main(){
check(1,1,1);
check(2,1,2);
check(0,30,0);
check(0,0,1);
check(2,3,8);
check(8,0,1);
check(INT_MAX, 0, 1);
check(INT_MAX, 1, INT_MAX);
check(4,4,256);
return EXIT_SUCCESS;
}
