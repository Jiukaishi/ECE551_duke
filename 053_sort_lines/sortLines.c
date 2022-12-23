#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<assert.h>
//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}
void sort(FILE* f){
    if(f==NULL){
perror("Can not open file");
exit(EXIT_FAILURE);
}
    char** data= NULL;
    char* line = NULL;
    size_t i = 0;
    size_t sz = 0;
    while(getline(&line, &sz, f)>=0){
        data = realloc(data,(i+1)*sizeof(*data));
        data[i]=line;
        i++;
        line = NULL;
    }
    free(line);
    sortData(data, i);
    for(size_t cnt = 0; cnt<i;cnt++)
    {printf("%s", data[cnt]);
    free(data[cnt]);
    }
    free(data);
    if(fclose(f)!=0){                                                                                         
         perror("Fail to close the file");                                                                    
         exit(EXIT_FAILURE);                                                                                  
    }
    return;
}
int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
    FILE* f;
    if(argc==1){
         f = stdin;
         sort(f);
    return 0;}
    else{
        for(size_t i=1; i<argc;i++){
         f = fopen(argv[i], "r");
         sort(f);         
        }
    }
    return EXIT_SUCCESS;
}
