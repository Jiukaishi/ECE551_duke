#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "argiments not matched\n");
    exit(EXIT_FAILURE);
  }
  //init catarray obj.
  catarray_t * words = malloc(sizeof *words);
  words->arr = NULL;
  words->n = 0;
  //read words.txt file.
  strings_t * lines = read_f(argv[1]);
  for (size_t i = 0; i < lines->n; i++) {
    //parse every line of words.txt. Extract category and word and store them into catarray.
    parseWordline(words, lines->arr[i]);
  }
  printWords(words);
  //free catarray obj.
  free_catarray(words);
  //free strings_t obj.
  free_strings_t(lines);
  return 0;
}
