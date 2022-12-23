#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "argiments not matched\n");
    exit(EXIT_FAILURE);
  }
  //init catarray obj.
  catarray_t * words = malloc(sizeof *words);
  words->arr = NULL;
  words->n = 0;
  //read words.txt file.
  strings_t * lines = read_f(argv[1]);
  //init prev words
  category_t * prev_words = malloc(sizeof *prev_words);
  prev_words->words = NULL;
  prev_words->name = NULL;
  prev_words->n_words = 0;

  for (size_t i = 0; i < lines->n; i++) {
    //every line, extract category and name andstore them in catarray obj.
    parseWordline(words, lines->arr[i]);
  }
  //free the memory of strings_t obj.
  free_strings_t(lines);
  //read template file.
  lines = read_f(argv[2]);
  for (size_t i = 0; i < lines->n; i++) {
    //extract all the blanks information into blank_list obj.
    blank_list_t * blanks = find_blanks_in_line(lines->arr[i]);
    //print it out, replace blank with corresponding word. rm=0 for allowing reuse word..
    printline(lines->arr[i], blanks, words, prev_words, 0);
    //free blank_list obj.
    free_blanks_list(blanks);
  }
  //free all the allocated memory.
  free_strings_t(lines);
  free_catarray(words);
  free_category(prev_words);
  free(prev_words);

  return 0;
}
