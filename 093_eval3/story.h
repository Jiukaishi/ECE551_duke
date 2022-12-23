#ifndef STORY_H
#define STORY_H
#include "page.h"
#include <cstdlib>
class Story {
 protected:
    //story's directory
  std::string directory;
  //the index of all the normal pages' index in pages.
  std::vector<size_t> choosepage_index;
  //each line's content in story.txt
  std::vector<std::string> contents;
  //cur requests
  std::map<std::string, long> req;

 public:
  //all the pages in the story.
  std::vector<Page *> pages;
  Story() : directory(), choosepage_index(), contents(), req(), pages() {}
  Story(std::string dicts) :
      directory(dicts), choosepage_index(), contents(), req(), pages() {
    directory = directory + "/";
    std::string story_dict = directory + "story.txt";
    contents = read_file(story_dict.c_str());
  }
  //copy constructor
  Story(const Story & rhs) :
      directory(rhs.directory),
      choosepage_index(rhs.choosepage_index),
      contents(rhs.contents),
      req(rhs.req),
      pages(rhs.pages) {
    //implement deepcopy
  }
  Story& operator =(const Story& rhs){
    if(this!=&rhs){
        directory = rhs.directory;
        choosepage_index = rhs.choosepage_index;
        contents = rhs.contents;
        req = rhs.req;
        pages = rhs.pages;
    }
    return *this;
  }
  virtual ~Story() {
    for (size_t i = 0; i < pages.size(); i++) {
      delete pages[i];
    }
  }
  bool check_valid_3a();
  bool check_valid_3b();
  bool check_valid_3c();
  bool check_valid();
  void parse_var(std::string line);
  void parse_choice(std::string line);
  void parse_page(std::string line, int at_idx);
  Page * display_and_proceed(Page * cur);
  void interact();
  void parse_line(std::string line);
  bool is_blank_line(std::string s);
  void parse_each_line();
  void print_pages();
  void path_helper(std::vector<size_t> &,
                   std::vector<size_t> &,
                   size_t,
                   std::vector<std::vector<size_t> > &,
                   std::vector<std::vector<size_t> > &);
  void find_path();
  size_t get_number(std::string);
};
#endif
