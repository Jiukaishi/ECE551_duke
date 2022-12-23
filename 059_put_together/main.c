#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  FILE* f = fopen(filename, "r");
  if(f==NULL){
  perror("open failed");
  exit(EXIT_FAILURE);
  }
  counts_t* p = createCounts();
  char * name = NULL;
  ssize_t i=0;
  size_t sz=0;
  while((i=getline(&name, &sz, f))>=0){
    if(i>0 && name[i-1]=='\n'){
        name[i-1] = '\0';
    }
    addCount(p,lookupValue(kvPairs, name));
  }
  free(name);
  fclose(f);
  return p;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
 //read the key/value pairs from the file named by argv[1] (call the result kv)
    kvarray_t* kvs = readKVs(argv[1]);

 //count from 2 to argc (call the number you count i)
    for(size_t i=2;i<argc;i++){
        //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t* c = countFile(argv[i],kvs);
   char* name = computeOutputFileName(argv[i]);
    FILE* f = fopen(name, "w");
   if (f == NULL) {
       perror("can't open file");
    exit(EXIT_FAILURE);
   }
   printCounts(c,f);
   fclose(f);
   free(name);
   freeCounts(c);
    }
  freeKVs(kvs);
  return EXIT_SUCCESS;
}
