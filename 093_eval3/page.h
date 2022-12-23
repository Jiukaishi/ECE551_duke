#ifndef PAGE_H
#define PAGE_H
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "errors.h"

inline std::vector<std::string> read_file(const char * filename) {
  std::vector<std::string> contents;
  std::ifstream reader(filename);
  if (reader.good() != true) {
    throw FopenError();
  }
  while (reader.eof() == false) {
    std::string tmp;
    getline(reader, tmp);
    contents.push_back(tmp);
  }
  reader.close();
  return contents;
}

class Page{
 protected:
  std::vector<std::string> contents;

 public:
  size_t pnumber;
  size_t type;  //0 for base, 1 for win, 2 for lose, 3 for choosing
  Page() : contents(), pnumber(-1), type(0) {}
  Page(std::vector<std::string> cs, size_t pn) :
      contents(cs), pnumber(pn), type(0) {}
  Page(const Page & rhs) : 
      contents(rhs.contents), pnumber(rhs.pnumber), type(rhs.type) {}
  Page & operator = (const Page& rhs){
    if(this!=&rhs){
        contents = rhs.contents;
        pnumber = rhs.pnumber;
        type = rhs.type;
    }
    return *this;
  }
  virtual ~Page() { contents.clear(); }
  virtual std::vector<size_t> get_destPages() {
    //only choosing page has destPages.
      throw WrongVirtualError();
    std::vector<size_t> tmp;
    return tmp;
  };
  virtual std::vector<std::pair<std::string, long> > get_req() {
      //only choosing page has req
    throw WrongVirtualError();
    return std::vector<std::pair<std::string, long> >();
  }
  virtual std::vector<bool> print_output(std::map<std::string, long> m){ return std::vector<bool>(); }
  virtual void add_choice(size_t dest_number, std::string content) {
      //only choosing page can add choice
    throw WrongVirtualError();
  }
  //only choosing page has these properties.
  virtual void add_req(std::string var, long v){throw WrongVirtualError();}
  virtual void add_has_prereq(bool has){throw WrongVirtualError();}
  virtual void add_prereq(std::string var, long v){throw WrongVirtualError();}
};
class WinPage : public Page {
 public:
  WinPage() : Page() { Page::type = 1; }
  WinPage(std::vector<std::string> cs, size_t pn) : Page(cs, pn) { Page::type = 1; }
  WinPage(const WinPage & rhs) :
      Page(rhs) {}  //type is assigned by parent copy constructor 
  WinPage & operator = (const WinPage& rhs){
    if(this!=&rhs){
        Page::operator = (rhs);
    }
    return *this;
  }
  virtual ~WinPage() {}
  virtual std::vector<bool> print_output(std::map<std::string, long> m){
    for (size_t i = 0; i < Page::contents.size(); i++) {
      std::cout << contents[i] << std::endl;
    }
    std::cout<<std::endl;
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
    return std::vector<bool> ();
  }
};
class LosePage : public Page {
 public:
  LosePage() : Page() { Page::type = 2; }
  LosePage(std::vector<std::string> cs, size_t pn) : Page(cs, pn) { Page::type = 2; }
  LosePage(const LosePage & rhs) : Page(rhs) {}
  LosePage & operator = (const LosePage& rhs){
    if(this!=&rhs){
        Page::operator = (rhs);
    }
    return *this;
  }
  virtual ~LosePage() {}
  virtual std::vector<bool> print_output(std::map<std::string, long> m){
    for (size_t i = 0; i < Page::contents.size(); i++) {
      std::cout << contents[i] << std::endl;
    }
    std::cout<<std::endl;
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
    return std::vector<bool> ();
  }
};
class ChoosePage : public Page {
 public:
    //change corrsponding var and its value when entered this page. 
  std::vector<std::pair<std::string, long> > req;
  std::vector<bool> has_prereq;
  //for each choice whoce has_prereq[i] is true, we store the prerequests it needs to be available.
  std::vector<std::pair<std::string, long> > prereq;
  //store each choice's destination page number.
  std::vector<size_t> pageNumbers;
  std::vector<std::string> choice_contents;
  ChoosePage() : 
      Page(), req(), has_prereq(), prereq(), pageNumbers(), choice_contents() {
    Page::type = 3;
  }
  ChoosePage(std::vector<std::string> cs, size_t pn) :
      Page(cs, pn), req(), has_prereq(), prereq(), pageNumbers(), choice_contents() {
    Page::type = 3;
  }
  ChoosePage(const ChoosePage & rhs) :
      Page(rhs),
      req(rhs.req),
      has_prereq(rhs.has_prereq),
      prereq(rhs.prereq),
      pageNumbers(rhs.pageNumbers),
      choice_contents(rhs.choice_contents) {}  
 ChoosePage & operator = (const ChoosePage& rhs){
    if(this!=&rhs){
        Page::operator = (rhs);
        req = rhs.req;
        has_prereq = rhs.has_prereq;
        prereq = rhs.prereq;
        pageNumbers = rhs.pageNumbers;
        choice_contents = rhs.choice_contents;
    }
    return *this;
 }
 virtual void add_choice(size_t dest_number, std::string content) {
    pageNumbers.push_back(dest_number);
    choice_contents.push_back(content);
  }
  virtual std::vector<bool> print_output(std::map<std::string, long> m) {
    for (size_t i = 0; i < Page::contents.size(); i++) {
      std::cout << Page::contents[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "What would you like to do?" << std::endl;
    std::cout << std::endl;
    std::vector<bool> is_available(choice_contents.size(), true);
    for (size_t i = 0; i < choice_contents.size(); i++) {
      if (has_prereq[i] == false) {
          //if it does not have any prereq, just print it out.
        std::cout << " " << i + 1 << ". " << choice_contents[i] << std::endl;
      }
      else {
          //if the var is in the map and its value meet the requirements, print it
        if (m.find(prereq[i].first) != m.end() &&
            m[prereq[i].first] == prereq[i].second) {
          std::cout << " " << i + 1 << ". " << choice_contents[i] << std::endl;
        }
        //otherwise, it is not availble at this time.
        else {
          std::cout << " " << i + 1 <<". "<<"<UNAVAILABLE>" << std::endl;
          is_available[i] = false;
        }
      }
    }
    //return the is_available to help judge if the user's input is an available choice.
    return is_available;
  }
  virtual void add_req(std::string var, long v) {
    //add an element in the req
    std::pair<std::string, long> tmp;
    tmp.first = var;
    tmp.second = v;
    req.push_back(tmp);
    return;
  }
  virtual void add_has_prereq(bool has){
    //add one element in the has_prereq
    has_prereq.push_back(has);
    return ;
  }
  virtual void add_prereq(std::string var, long v){
    //add one element in the prereq.
    std::pair<std::string, long> tmp;
    tmp.first = var;
    tmp.second = v;
    prereq.push_back(tmp);
    return;
       
  }
  virtual std::vector<std::pair<std::string, long> > get_req() { return req; }
  virtual std::vector<size_t> get_destPages() { return pageNumbers; }
  virtual ~ChoosePage() {}
};
#endif
