#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
size_t maxSeq(int *array, size_t n);
void test(int* array, size_t n, size_t res){
    size_t ans = maxSeq(array, n);
    if(res!=ans)
    {   printf("%d, %zu", array[0], n );
        exit(EXIT_FAILURE);
    }}
int main(){
    int array6[3] = {2,2,2};
    test(array6,3,1);
    int array[3] = {0,1,2};
    test(array, 3, 3);
    int array1[1] = {0};
    test(array1,1,1);
    int array2[10] = {1,2,1,3,5,7,2,4,6,9};
    test(array2,10,4);
    int array3[3] = {INT_MIN,0,INT_MAX};
    test(array3,3,3);
    int array4[6]={1,2,3,4,4,5};
    test(array4,6,4);
    int* array5=NULL;
    
    test(array5, 0,0 );
    printf("success");
    return 0;

}
