#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
size_t result = 0;
size_t recursion(int* array, size_t n, size_t curr, size_t prev){
    if(curr == n)
        return 0;
    if(curr!=0 && array[curr]>array[curr-1]){
        prev = prev+1;
    
    }else{
        prev = 1;
    }
    if(result<prev)
        result = prev;
    recursion(array, n, curr+1, prev);
    return prev;
}
size_t maxSeq(int* array, size_t n){
    result=0;
    if(n==0)
        return 0;
    recursion(array, n, 0,0);
    return result;

}
