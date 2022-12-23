#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__
#include "assert.h"
#include "provided.h"
#include <string.h>
#include <stdio.h>
//any functions you want your main to use
struct strings_tag{
        char** arr;
        size_t n;
};
typedef struct strings_tag strings_t;

struct blanks_tag{
        char* fieldname;
        size_t l;
        size_t r;
};
typedef struct blanks_tag blanks_t;

struct blank_list_tag{
        blanks_t* arr;
        size_t n;
};
typedef struct blank_list_tag blank_list_t;
strings_t* read_f(const char* filename);
blank_list_t* find_blanks_in_line(const char* line);
const char* mychooseWord(char* cat, category_t* prev_words, catarray_t* cats, int mv);
void printline(char* line, blank_list_t* blanks, catarray_t* cats, 
               category_t* prev_words, int mv);

int is_valid_int(const char* ptr, category_t* prev_words );
void parseWordline(catarray_t* words, const char* line);
void free_strings_t(strings_t* s);
void free_catarray(catarray_t* ptr);
void free_category(category_t* ptr);
void free_blanks_list(blank_list_t* ptr);

void remove_word_from_cats(const char* cat, const char* word, catarray_t* cats);
#endif
