#include "function.h"
#include <iostream>

int binarySearchForZero(Function<int, int> * f, int low, int high){
    if(f->invoke(low)>0){
        return low;
    }
    int  l = low;
    while(low<high){
        int mid = low+(high-low)/2;
        int v = f->invoke(mid);
        if(v<=0)
            low = mid+1 ;
        else
            high = mid;
    }
    if(low==l)
        return low;
    return low-1;


}
