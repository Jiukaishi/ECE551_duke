#include "story.h"
bool Story::check_valid_3a() {
  bool is_valid = true;
  for (size_t i = 0; i < choosepage_index.size(); i++) {
    //iterate all the normal pages: check their choices' destinations.
    Page * cur = pages[choosepage_index[i]];
    std::vector<size_t> pageNumbers = cur->get_destPages();
    //iterate over all the destination pages.
    for (size_t j = 0; j < pageNumbers.size(); j++) {
      size_t destination_pnumber = pageNumbers[j];  
      //check if the destination page is a valid page.
      bool destination_in_pages = false;
      for (size_t k = 0; k < pages.size(); k++) {
        if (pages[k]->pnumber == destination_pnumber) {
          destination_in_pages = true;
          break;
        }
      }
      if (destination_in_pages == false) {
        is_valid = false;
        return is_valid;
      }
    }
  }
  return is_valid;
}
bool Story::check_valid_3b() {
    //referenced[i] means the i-th page is reachable or not.
  std::vector<bool> referenced(pages.size(), false);
  referenced[0] = true;  //except page 0;
  //iterate over all the choosing pages' destination numbers.
  for (size_t i = 0; i < choosepage_index.size(); i++){
    Page * cur = pages[choosepage_index[i]];
    std::vector<size_t> pageNumbers = cur->get_destPages();
    for (size_t j = 0; j < (pageNumbers).size(); j++) {
      referenced[pageNumbers[j]] = true;
    }
  }

  for (size_t i = 0; i < referenced.size(); i++) {
    if (referenced[i] == false) {
      return false;
    }
  }

  return true;
}

bool Story::check_valid_3c() {
  bool has_win_page = false;
  bool has_lose_page = false;
  for (size_t i = 0; i < pages.size(); i++){
    if ((pages[i])->type == 1) {
      has_win_page = true;
    }
    if ((pages[i])->type == 2) {
      has_lose_page = true;
    }
  }
  return has_win_page && has_lose_page;
}

bool Story::check_valid(){
  if (check_valid_3a() && check_valid_3b() && check_valid_3c()) {
    return true;
  }
  return false;
}

void Story::parse_choice(std::string line){
//locate the two : index.
  int first_col = -1, second_col = -1;
  size_t tmp = -1;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == ':') {
      tmp = i;
      first_col = i;
      break;
    }
  }
  for (size_t i = tmp + 1; i < line.length(); i++) {
    if (line[i] == ':') {
      second_col = i;
      break;
    }
  }
  //there is only 1 :, throw an error.
  if (first_col == -1 || second_col == -1) {
    throw ContentError();
  }

  std::string pnumber = line.substr(0, first_col);
  std::string dnumber = line.substr(first_col + 1, second_col - (first_col + 1));
  std::string choice_content =
      line.substr(second_col + 1, line.length() - second_col - 1);
  size_t lb = pnumber.find("["), rb=pnumber.find("]");
  size_t pn;
  //check if it contains prereq
  if(rb==std::string::npos||lb==std::string::npos){
    pn = get_number(pnumber);
  }else{
    pn = get_number(pnumber.substr(0, lb));
  }
  size_t dn = get_number(dnumber);
  //check if destination number and start number are already existed (have been declared)
 // bool dn_in_pages = false;
  Page* cur = NULL;
  bool matched = false;
  for (size_t i = 0; i < pages.size(); i++) {
    cur = pages[i];
    if (cur->pnumber == pn) {
      cur->add_choice(dn, choice_content);
      matched = true;
      break;
    }
  }
  if (matched == false) {
    throw ContentError();
  }
  std::pair<std::string, long> pr;
  //check if contains prereq (for step4)
  size_t lbrace = pnumber.find("[");
  size_t rbrace = pnumber.find("]");
  bool has_pre = false;
  if (lbrace != std::string::npos && rbrace != std::string::npos && lbrace < rbrace) {
    std::string prereqest = pnumber.substr(lbrace + 1, rbrace - (lbrace + 1));
    int eq_idx = -1;
    for (size_t i = 0; i < prereqest.length(); i++) {
      if (prereqest[i] == '=') {
        eq_idx = i;
        break;
      }
    }
    if (eq_idx == -1){
        throw Step4Error();
    }
    std::string var = prereqest.substr(0, eq_idx);
    //if the var has not been defined in the map, we init it and set its value to 0
    if (req.find(var) == req.end()) {
      req[var] = 0;
    }
    long v = std::atol(
        prereqest.substr(eq_idx + 1, prereqest.length() - (eq_idx + 1)).c_str());
    pr.first = var;
    pr.second = v;
    has_pre = true;
  }
  cur->add_has_prereq(has_pre);
  cur->add_prereq(pr.first, pr.second);
  return;
}
void Story::parse_var(std::string line){
//for page$var=value, we need to make sure it has a dollar sign and an
//equation, otherwise throw error.
    int dollar_idx = -1, eq_idx = -1;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == '$') {
      dollar_idx = i;
      break;
    }
  }
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == '=') {
      eq_idx = i;
      break;
    }
  }
  if (dollar_idx == -1 || eq_idx == -1){
    throw Step4Error();
  }
  std::string pnumber = line.substr(0, dollar_idx);
  std::string var = line.substr(dollar_idx + 1, eq_idx - (dollar_idx + 1));
  std::string value = line.substr(eq_idx + 1, line.length() - (eq_idx + 1));
  long v = std::atol(value.c_str());
  size_t pn = get_number(pnumber);
  //check if the pagenumber pn is available (declared before)

  bool matched = false;
  Page* cur = NULL;
  for (size_t i = 0; i < pages.size(); i++) {
    cur = pages[i];
    if (cur->pnumber == pn) {
        //if so, add to its req.
      cur->add_req(var, v);
      matched=true;
      break;
    }
  }
  if (matched == false) {
    throw ContentError();
  }
  return;
}
void Story::parse_page(std::string line, int at_idx) {
 //check if there is a :. If it only has a $, we consider it is not a valid line.
  int col_idx = -1;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == ':') {
      col_idx = i;
      break;
    }
  }
  if (col_idx == -1) {
    throw ContentError();
  }
  std::string pnumber = line.substr(0, at_idx);
  std::string ptype = line.substr(at_idx + 1, col_idx - (at_idx + 1));
  std::string filename = line.substr(col_idx + 1, line.length() - col_idx - 1);
  size_t pn = get_number(pnumber);
  if (ptype[0] == 'N') {
  // a normal page
      filename = directory + filename;
    std::vector<std::string> contents = read_file(filename.c_str());
    Page * page = new ChoosePage(contents, pn);
    //page declaration must appear in order:
   if(pages.size()>0 && page->pnumber!=(pages[pages.size()-1])->pnumber+1){
      throw ContentError();
  }
    pages.push_back(page);
    choosepage_index.push_back(pages.size() - 1);
  }
  else if (ptype[0] == 'W') {
    filename = directory + filename;
    std::vector<std::string> contents = read_file(filename.c_str());
    Page * page = new WinPage(contents, pn);
   if(pages.size()>0 && page->pnumber!=(pages[pages.size()-1])->pnumber+1){
      throw ContentError();
  }
    pages.push_back(page);
  }
  else if (ptype[0] == 'L') {
    filename = directory + filename;
    std::vector<std::string> contents = read_file(filename.c_str());
    Page * page = new LosePage(contents, pn);
   if(pages.size()>0 && page->pnumber!=(pages[pages.size()-1])->pnumber+1){
      throw ContentError();
  }
    pages.push_back(page);
  }
  else {
    throw TypeError();
  }
  return;
}
Page * Story::display_and_proceed(Page * cur){

  //choosing page type is 3. if not 3, it is a either win or lose page.
  
  std::vector<bool> is_avail;
    if (cur->type != 3) {
        cur->print_output(req);
        return NULL;
  }else{
  //for step4, we build a page_req vector, each element is a pair
  //<std::string, long>: string is the var and long is the value.
  std::vector<std::pair<std::string, long> > page_req = cur->get_req();
  //req
  for (size_t i = 0; i < page_req.size(); i++) {
    //set current story's req according to current page's page_req.
    //see if current story's req has the key
    if (req.find(page_req[i].first) == req.end()) {
        //if not have, set it
      req[page_req[i].first] = page_req[i].second;
    }
    else {
        //if have, change it
      req[page_req[i].first] = page_req[i].second;
    }
  }
  //after setting, we input current state's vars and their values in req and
  //get output is_avail, which indicates each choice of that page is available
  //or not at this time.
  is_avail = cur->print_output(req);
  }

  while (1) {
    //get user input, only exit if user inputs a valid choice.
    std::string user_input;
    std::getline(std::cin, user_input);
    int input = std::atoi(user_input.c_str());
    size_t sized_input = std::atol(user_input.c_str());
    //check if the input number is valid:
    std::vector<size_t> pageNumbers = cur->get_destPages();
    if (input <= 0 || sized_input > pageNumbers.size()) {
      std::cout << "That is not a valid choice, please try again" << std::endl;
    }
    else if (is_avail[input - 1] == false) {
      std::cout << "That choice is not available at this time, please try again"
                << std::endl;
    }
    else {
      size_t destination_number = pageNumbers[input - 1];
      return pages[destination_number];
    }
  }
  return cur;
}
void Story::interact(){
//used in step2 and step4
  //start from page 0
  Page * cur = pages[0];
  while(1){
      //loop infinitly unless display_and_proceed() exits.
    cur = display_and_proceed(cur);
    //if display_and_proceed() encounter win or lose page, it returns NULL. 
    if (!cur){
      return;
    }
  }
}
void Story::parse_line(std::string line) {
//check if this line is a page declaration line.
    int at_idx = -1;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == '@') {
      at_idx = i;
      break;
    }
  }
  //check if there is a : in the line
  int col_idx = -1;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == ':') {
      col_idx = i;
      break;
    }
  }
  //if there is no @ and : in the line, I consider it as a var declariation.
  if (at_idx == -1 && col_idx == -1) {
    parse_var(line);
  }
  //if there is no @ but is :, it may be a choice
  else if (at_idx == -1) {
    parse_choice(line);
  }
  else {
      //if there is both @ and :, it is a page declaration line.
    parse_page(line, at_idx);
  }
  return;
}
size_t Story::get_number(std::string s) {
  //transform a string to a size_t number
    if (s.length() == 0) {
    throw NumberError();
  }
  
  bool sign_checked = false;
  //check if this string contains letter not in [0-9]
  for (size_t i = 0; i < s.length(); i++) {
    if (std::isspace(s[i]) != 0) {
        //igonre the whitespace at the front
      continue;
    }else if (std::isdigit(s[i])!=0){
    continue;    
    }else if(s[i]=='-'&& sign_checked==false){
        //if we meet a sign for the first time, we consider it is legal.
        sign_checked = true;
    }
    else{
        throw NumberError();
    }
  }
  //if the number is a negative number, throw error.
  int signed_trans = std::atoi(s.c_str());

  if(signed_trans==0){return 0; }
  if(signed_trans<0){throw NumberError();}
  unsigned long long pn = std::strtoull(s.c_str(),NULL,10);
  size_t trans, trans_add1;
  trans = pn, trans_add1 = pn+1;
  //check if the number overflows.
  if(trans<trans_add1){
    return trans;
  }else{
        throw NumberError();
  }
  return trans;
}
bool Story::is_blank_line(std::string s) {
//check if a line is a blank line
//I consider a line is a blank if
//1. It has 0 chars.
//2. all of its chars is whitespace.
  if (s.length() == 0) {
    return true;
  }
  for (size_t i = 0; i < s.length(); i++) {
    if (std::isspace(s[i]) == 0) {
      return false;
    }
  }
  return true;
}
void Story::parse_each_line() {
  for (size_t i = 0; i < contents.size(); i++) {
      //parse each line of the story if this line is not a blank line.
    if (!is_blank_line(contents[i])) {
      parse_line(contents[i]);
    }
  }
  return;
}
void Story::print_pages() {
    //for step1, iterate over the pages in self.pages and call each page's print_output method.
  for (size_t i = 0; i < pages.size(); i++) {
    std::cout << "Page " << pages[i]->pnumber << std::endl;
    std::cout << "==========" << std::endl;
    pages[i]->print_output(req);
  }
  return;
}
void Story::path_helper(std::vector<size_t> & absolute_path,
                        std::vector<size_t> & relative_path,
                        size_t cur_page,
                        std::vector<std::vector<size_t> > & absolute_choice,
                        std::vector<std::vector<size_t> > & relative_choice) {
    //dfs finding path
  Page * pg = pages[cur_page];
  if (pg->type != 3) {
      //if current page is a win or lose page.
    if (pg->type == 1) {
        //if it is a win page. than it means we find a valid path. Store it and exit.
      absolute_choice.push_back(absolute_path);

      relative_choice.push_back(relative_path);
    }
    return;
  }
  else {
    std::vector<size_t> pn = pg->get_destPages();
    for (size_t i = 0; i < pn.size(); i++) {
      bool inPath = false;
      for (size_t j = 0; j < absolute_path.size(); j++) {
        //if current node is already in the path, we abort it.
          if ((pn[i]) == absolute_path[j]) {
          inPath = true;
          break;
        }
      }
      if (inPath) {
        continue;
      }
      //if it is a new page, add to the global path.
      else {
        absolute_path.push_back(pn[i]);
        //remember the choice in relative_path
        relative_path.push_back(i + 1);
        path_helper(
            absolute_path, relative_path, pn[i], absolute_choice, relative_choice);
        absolute_path.pop_back();
        relative_path.pop_back();
      }
    }
  }
}
void Story::find_path() {
    //absolute path stores all the page number on the current path
    //relative path stores the chosen choice's index of each page number on the path.
  std::vector<std::vector<size_t> > absolute_choice, relative_choice;
  std::vector<size_t> absolute_path, relative_path;
  //start from page 0
  absolute_path.push_back(0);
//call the helper function which recursively traverse the graph
  path_helper(absolute_path, relative_path, 0, absolute_choice, relative_choice);
  //print paths
  for (size_t i = 0; i < absolute_choice.size(); i++) {
    for (size_t j = 0; j < absolute_choice[i].size(); j++) {
    //special print format for the last page in path.
      if (j == absolute_choice[i].size() - 1) {
        std::cout << absolute_choice[i][j] << "("
                  << "win"
                  << ")" << std::endl;
      }
      else {
        std::cout << absolute_choice[i][j] << "(" << relative_choice[i][j] << "),";
      }
    }
  }
}
