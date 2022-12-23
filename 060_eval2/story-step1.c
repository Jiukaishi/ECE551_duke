#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "argiments not matched\n");
    exit(EXIT_FAILURE);
  }
  //read from file.
  strings_t * lines = read_f(argv[1]);
  for (size_t i = 0; i < lines->n; i++) {
    //find blank in every file.
    blank_list_t * blanks = find_blanks_in_line(lines->arr[i]);
    //print the result.
    printline(lines->arr[i], blanks, NULL, NULL, 0);
    //free the blank_list obj.
    free_blanks_list(blanks);
  }
  //free strings_t obj.
  free_strings_t(lines);
  return 0;
}
