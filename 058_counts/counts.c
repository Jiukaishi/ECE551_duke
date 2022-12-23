#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
    counts_t* p = malloc(sizeof(*p));
    p->sz = 0;
    p->list = NULL;
    p->unknown_ct = 0;
    return p;
}
void addCount(counts_t * c, const char * name) {
    if(!name){
        c->unknown_ct+=1;
    }
    else{
        size_t matched = 0;
        for(size_t i=0;i<c->sz;i++){
            if(strcmp((c->list)[i].name, name)==0){
                matched=1;
                (c->list)[i].time+=1;
                break;
            }
        }
        if(!matched){
            c->sz ++;
            if(!c->list){
                c->list = malloc(sizeof(* (c->list)));
            }else{
                c->list = realloc(c->list, (c->sz)* sizeof(*c->list) );
            }
            c->list[c->sz-1].name =  strdup(name);
            c->list[c->sz -1].time = 1;
        }
    }
    return;
}
void printCounts(counts_t * c, FILE * outFile) {
    for(size_t i=0; i<c->sz;i++){
        fprintf(outFile, "%s: %zu\n", c->list[i].name, c->list[i].time);
    }
    if(c->unknown_ct!=0){
        fprintf(outFile, "<unknown>: %zu\n", c->unknown_ct);    
    }
    return;
}

void freeCounts(counts_t * c) {
    for(size_t i=0;i<c->sz;i++){
        free(c->list[i].name);
    }
    free(c->list);
    free(c);
    return;
}


