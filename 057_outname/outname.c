#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"
#include <assert.h>
char * computeOutputFileName(const char * inputName) {
    assert(inputName);
    const char * p = inputName;
    size_t i=0;
    while(*p!='\0'){
        i++;
        p++;
    }
    size_t j = i;
    char* append =malloc((i+8)*sizeof(*append));
    while(i!=0){
    append[i-1]=inputName[i-1];
    i--;
    }
    const char* s = ".counts";
    while(*s!='\0'){
        append[j]=*s;
        s++;
        j++;
    }
    append[j]=*s;
    return append;
}
