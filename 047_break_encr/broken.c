#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int encrypt(FILE * f){
  int count[26]={0};
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      c -= 'a';
      count[c]++;
    }
  }
  int max_idx = 0;
  for(int i=0; i< 26;i++){
    if(count[max_idx]<count[i])
        max_idx = i;
  }
  /*
  for(int i=0; i<max_idx;i++){
      if(count[i]==count[max_idx]){
          perror("More than 1 most common letter");
          exit(EXIT_FAILURE);
      }
  }
  if(count[max_idx]==0){
      perror("No English letter");
      exit(EXIT_FAILURE);
  }
  */
  return (max_idx+26-4)%26; 
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr,"Usage: breaker inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int i = encrypt(f);
  fprintf(stdout,"%d\n",i);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
   return 0;
}
