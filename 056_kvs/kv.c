#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"



kvarray_t * readKVs(const char * fname) {
    FILE* f = fopen(fname,"r");
    if (f == NULL) {
        fprintf(stderr, "failed to open file %s\n", fname);
        exit(EXIT_FAILURE);
        }
    char** data = NULL;
    char* line = NULL;
    size_t i=0, sz=0;
    while(getline(&line,&sz,f)>=0){
        data = realloc(data, (i+1)*sizeof(*data));
        data[i]=line;
        i++;
        line=NULL;
    }
    free(line);
    if(fclose(f)!=0){
        perror("close file failed");
        exit(EXIT_FAILURE);
    }

    kvarray_t* kvarray=malloc(sizeof(*kvarray));
    kvpair_t* kvs=NULL;
    for(size_t k=0;k<i;k++){
        const char* p = data[k];
        kvs = realloc(kvs,(k+1)*sizeof(*kvs));
        const char* vp = strchr(p,'=');
        const char* kp = strchr(++vp, '\n');
        if(!vp){
            perror("False");    
            exit(EXIT_FAILURE);
        }
          if (kp == NULL) {
            perror("Input error");
            exit(EXIT_FAILURE);
        }
        kvs[k].value = strndup(vp, (size_t) (kp - vp));
        kvs[k].key = strndup(p,(size_t)(--vp-p));
        free(data[k]);
    }
    free(data);
    kvarray->kvs = kvs;
    kvarray->length = i;
    return kvarray;

}

void freeKVs(kvarray_t * pairs) {
    for(size_t i=0;i<pairs->length;i++){
        free(pairs->kvs[i].value);
        free(pairs->kvs[i].key);
    }
    free(pairs->kvs);
    free(pairs);
    return;
}

void printKVs(kvarray_t * pairs) {
  for(size_t i=0;i<pairs->length;i++){
        printf("key = '%s' value = '%s'\n",pairs->kvs[i].key, pairs->kvs[i].value);
    }
  return;
}
char * lookupValue(kvarray_t * pairs, const char * key) {
        size_t i;
          kvpair_t *p;

            for (i = 0; i < pairs->length; i++) {
                    p = &pairs->kvs[i];
                        if (strcmp(key, p->key) == 0) {
                                  return p->value;
                                      }
                          }
              return NULL;

}
