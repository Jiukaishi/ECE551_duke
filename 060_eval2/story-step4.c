#include "rand_story.h"
int main(int argc, char ** argv) {
  //check arguments. Either 3 or 4.
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "argiments not matched\n");
    exit(EXIT_FAILURE);
  }
  //declare pointer to words file and template file.
  const char * word_file;
  const char * template_file;
  //rm is 0 if no "-n" in the arguments.
  int rm = 0;
  if (argc == 4) {
    //check if the first argument is "-n".
    if (strcmp(argv[1], "-n") != 0) {
      fprintf(stderr, "expected -n but got %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    //if it is, set rm to 1.
    rm = 1;
    word_file = argv[2];
    template_file = argv[3];
  }
  else {
    //if there are 3 arguments, no reuse restrictions. rm to be 0.
    rm = 0;
    word_file = argv[1];
    template_file = argv[2];
  }
  //init catarray obj.
  catarray_t * words = malloc(sizeof *words);
  words->arr = NULL;
  words->n = 0;
  //read words.txt to strings_t obj.
  strings_t * lines = read_f(word_file);
  //init prev words for checking backreference.
  category_t * prev_words = malloc(sizeof *prev_words);
  prev_words->words = NULL;
  prev_words->name = NULL;
  prev_words->n_words = 0;
  //parse every line in words.txt, extracting cat name and word name. storing them in catarray.
  for (size_t i = 0; i < lines->n; i++) {
    parseWordline(words, lines->arr[i]);
  }
  //free the strings_t obj for reading next file.
  free_strings_t(lines);
  //read template file.
  lines = read_f(template_file);
  for (size_t i = 0; i < lines->n; i++) {
    //extracting every blank's information line by line.
    blank_list_t * blanks = find_blanks_in_line(lines->arr[i]);
    //print the result to the stdout.
    printline(lines->arr[i], blanks, words, prev_words, rm);
    //free this line's blank information because we have used it.
    free_blanks_list(blanks);
  }
  //free all the memory allocated before.
  free_strings_t(lines);
  free_catarray(words);
  free_category(prev_words);
  free(prev_words);

  return 0;
}
