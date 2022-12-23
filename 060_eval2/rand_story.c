#include "rand_story.h"
strings_t * read_f(const char * filename) {
  //read from file
  FILE * f = fopen(filename, "r");
  //if filename is not a valid file.
  if (f == NULL) {
    perror("Open file failed");
    exit(EXIT_FAILURE);
  }
  char ** data = NULL;
  char * line = NULL;
  size_t sz = 0, i = 0;
  while (getline(&line, &sz, f) >= 0) {
    data = realloc(data, (i + 1) * sizeof(*data));
    data[i] = line;
    line = NULL;
    i++;
  }
  //free last line.
  free(line);
  line = NULL;
  strings_t * res = malloc(sizeof(*res));
  res->arr = data;
  res->n = i;
  //close the file.
  if (fclose(f) != 0) {
    perror("close file failed");
    exit(EXIT_FAILURE);
  }
  return res;
}
blank_list_t * find_blanks_in_line(const char * line) {
  //read every blank in the line. Each blank is a black_t object in blank_list_t.arr.
  blanks_t * blank = NULL;
  size_t sz = 0;
  const char * ptr = line;
  int upper_idx = -1;
  // upper_idx is the front underscore's index.
  while (*ptr != '\0') {
    if (*ptr == '_') {
      if (upper_idx == -1) {
        //if this is the front underscore, store its index.
        upper_idx = ptr - line;
      }
      else {
        // we found an end underscore
        blank = realloc(blank, (sz + 1) * (sizeof(*blank)));
        blank[sz].l = upper_idx;   //front underscore index
        blank[sz].r = ptr - line;  //end underscore index
        blank[sz].fieldname =
            malloc((blank[sz].r - blank[sz].l) * sizeof(char));  //the category name
        char * cur = blank[sz].fieldname;
        //copy and add terminal in fieldname.
        for (size_t i = blank[sz].l + 1; i < blank[sz].r; i++) {
          *cur = line[i];
          cur++;
        }
        *cur = '\0';
        //reset front underscore index to -1 for next blank.
        upper_idx = -1;
        sz++;
      }
    }
    ptr++;
  }
  if (upper_idx != -1) {
    fprintf(stderr, "matching underscore must on the same line\n");
    exit(EXIT_FAILURE);
  }
  //allocate memory for blank_list.
  blank_list_t * blank_list = malloc(sizeof(*blank_list));
  blank_list->arr = blank;
  blank_list->n = sz;
  return blank_list;
}
int is_valid_int(const char * ptr, category_t * prev_words) {
  //check if the string is a valid integer.
  const char * p_input = ptr;
  if (*p_input == '\0') {
    return -1;
  }
  //ignore space before integer.
  while (*p_input == 9 || *p_input == 32) {
    p_input++;
  }
  //check every char is a digit, otherwise return -1 indicating false.
  while (*p_input != '\0') {
    if (*p_input >= '0' && *p_input <= '9') {
      p_input++;
    }
    else {
      return -1;
    }
  }
  //string to int
  int past_idx = atoi(ptr);
  if (past_idx == 0) {
    return -1;
  }
  //calculating the idx in prev_words.
  int idx = prev_words->n_words - past_idx;
  //if the index is out of bound, return false.
  if (idx < 0 || idx >= prev_words->n_words) {
    fprintf(stderr, "a valid integer but index out of range\n");
    exit(EXIT_FAILURE);
  }
  //if it is a valid integer, return its index.
  return idx;
}

const char * mychooseWord(char * cat,
                          category_t * prev_words,
                          catarray_t * cats,
                          int rm) {
  //choose the random or previous word. rm is 0 if no reuse limitation.
  char * wd = NULL;
  if (cat == NULL) {
    fprintf(stderr, "categories is null ptr\n");
    exit(EXIT_FAILURE);
  }
  if (cats == NULL) {
    //special case for step 1.
    return chooseWord(cat, cats);
  }
  if (prev_words == NULL) {
    fprintf(stderr, "Prev words points to NULL!\n");
    exit(EXIT_FAILURE);
  }
  // first check if cat is a valid category.
  int is_a_cat = 0;
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cat, cats->arr[i].name) == 0) {
      if (cats->arr[i].n_words > 0) {
        // if the number of words is less than 1 because of the reuse restriction, return false.
        is_a_cat = 1;
      }
    }
  }
  // check if it is a valid integer.
  int idx = is_valid_int(cat, prev_words);
  //if it is a valid integer, return the previous word
  //else if it is a category, choose a random word using the provided function chooseWord.
  //else, return false.
  if (idx != -1) {
    wd = strdup(prev_words->words[idx]);
  }
  else {
    if (is_a_cat == 0) {
      fprintf(stderr, "not a legal cat name\n");
      exit(EXIT_FAILURE);
    }
    else {
      wd = strdup(chooseWord(cat, cats));
      //rm==0 for step3 and rm!=0 for reuse restriction.
      if (rm != 0)
        remove_word_from_cats(cat, wd, cats);
    }
  }
  //update prev_words
  prev_words->words = realloc(prev_words->words,
                              (prev_words->n_words + 1) * (sizeof(*(prev_words->words))));
  prev_words->words[prev_words->n_words] = wd;
  prev_words->n_words++;
  return wd;
}
void remove_word_from_cats(const char * cat, const char * word, catarray_t * cats) {
  //remove the used word from category if "-n" is given in step 4.
  //find the category
  size_t cat_idx = -1;
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cats->arr[i].name, cat) == 0) {
      //if we find the category, change cat_idx from -1 to its cat index.
      cat_idx = i;
      break;
    }
  }
  //if not found, throw exception.
  if (cat_idx == -1) {
    fprintf(stderr, "categorie not found\n");
    exit(EXIT_FAILURE);
  }
  //find the word in its corrsponding category.words.
  size_t word_idx = -1;
  for (size_t i = 0; i < cats->arr[cat_idx].n_words; i++) {
    if (strcmp(cats->arr[cat_idx].words[i], word) == 0) {
      word_idx = i;
      break;
    }
  }
  //if this word is not found in its cat, throw exception.
  if (word_idx == -1) {
    fprintf(stderr, "word not found\n");
    exit(EXIT_FAILURE);
  }
  //rearrange the words.
  //for every word behind the removed word, move left.
  for (size_t i = word_idx; i < cats->arr[cat_idx].n_words - 1; i++) {
    free(cats->arr[cat_idx].words[i]);
    cats->arr[cat_idx].words[i] = strdup(cats->arr[cat_idx].words[i + 1]);
  }
  //free the last element's memory which is not included in the loop.
  free(cats->arr[cat_idx].words[cats->arr[cat_idx].n_words - 1]);
  cats->arr[cat_idx].n_words--;
  //shrink the words's memory using realloc
  cats->arr[cat_idx].words =
      realloc(cats->arr[cat_idx].words, cats->arr[cat_idx].n_words * (sizeof(char *)));
  return;
}
void printline(char * line,
               blank_list_t * blanks,
               catarray_t * cats,
               category_t * prev_words,
               int rm) {
  size_t cur_idx = 0;        //current index of the word in the line.
  size_t cur_blank_idx = 0;  //current index of the blank
  while (line[cur_idx] != '\0') {
    //iterate over every word in the line.
    if (cur_blank_idx < blanks->n) {
      //if there are blanks have not printed yet.
      blanks_t cur_blank = (blanks->arr)[cur_blank_idx];
      if (cur_blank.l == cur_idx) {
        //if we are on the front underscore of this blank.
        //print the corrsponding word in category or previous word.
        //ignore all the words between and include the front and end underscore.
        cur_idx = cur_blank.r + 1;
        cur_blank_idx++;
        printf("%s", mychooseWord(cur_blank.fieldname, prev_words, cats, rm));
      }
      else {
        //if it is not in an blank, just print the word.
        printf("%c", line[cur_idx]);
        cur_idx++;
      }
    }
    else {
      //if all the blanks are printed, print the rest of the line and quit.
      printf("%s", line + cur_idx);
      break;
    }
  }
  return;
}
void parseWordline(catarray_t * words, const char * line) {
  //parse every line in words.txt.
  const char * ptr = line;
  char *cat_name = NULL, *word_name = NULL;
  char * end_ptr = strchr(line, '\n');
  while (*ptr != '\0' && *ptr != '\n') {
    if (*ptr == ':') {
      //if we found the first :, copy the words before it as category name and words after it as word name.
      cat_name = strndup(line, ptr - line);
      word_name = strndup(ptr + 1, end_ptr - 1 - ptr);
      break;
    }
    ptr++;
  }
  //if catname is NULL, it means we did not find a :.
  if (cat_name == NULL) {
    fprintf(stderr, "No colum found\n");
    exit(EXIT_FAILURE);
  }
  // put the word names into corrsponding category.
  for (size_t i = 0; i < words->n; i++) {
    if (strcmp(words->arr[i].name, cat_name) == 0) {
      words->arr[i].words =
          realloc(words->arr[i].words,
                  (words->arr[i].n_words + 1) * sizeof(*(words->arr[i].words)));
      words->arr[i].words[words->arr[i].n_words] = word_name;
      free(cat_name);
      cat_name = NULL;
      words->arr[i].n_words++;
      break;
    }
  }
  //if there is no corresponding category, create a new category for it.
  if (cat_name != NULL) {
    words->arr = realloc(words->arr, (words->n + 1) * sizeof(*(words->arr)));
    words->arr[words->n].name = strdup(cat_name);
    words->arr[words->n].words = malloc(sizeof(*(words->arr[words->n].words)));
    words->arr[words->n].words[0] = word_name;
    words->arr[words->n].n_words = 1;
    words->n++;
    free(cat_name);
  }
  return;
}

void free_strings_t(strings_t * s) {
  //free a string_t object.
  if (!s) {
    fprintf(stderr, "empty string_t\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < s->n; i++) {
    free(s->arr[i]);
  }
  free(s->arr);
  free(s);
  return;
}
void free_category(category_t * ptr) {
  //free a category_t obj. But do not free the ptr itself.
  for (int i = 0; i < ptr->n_words; i++) {
    free(ptr->words[i]);
  }
  free(ptr->words);
  free(ptr->name);
  return;
}
void free_catarray(catarray_t * ptr) {
  //free a catarray_t object.
  for (size_t i = 0; i < ptr->n; i++) {
    free_category(ptr->arr + i);
  }
  free(ptr->arr);
  free(ptr);
  return;
}
void free_blanks_list(blank_list_t * ptr) {
  //free a black_list_t obj.
  if (!ptr) {
    fprintf(stderr, "empty ptr\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < ptr->n; i++) {
    free(ptr->arr[i].fieldname);
  }
  free(ptr->arr);
  free(ptr);
}
