#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <vector>
class FopenError: public std::exception{
    const char *what(){return "open file failed";}
};
class WrongVirtualError: public std::exception{
    const char* what(){return "Dynamic Dispatched Error";}
};
class ContentError: public std::exception{
    const char *what(){return "Content not matched the template";}
};
std::vector<std::string> read_file(const char* filename){
    std::vector<std::string> contents;
    std::ifstream reader(filename);
    if(reader.good()!=true){
        throw FopenError();
    }
    while(reader.eof()==false){
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
    int pnumber;
    Page():contents(),pnumber(-1){}
    Page(std::vector<std::string> cs, int pn): contents(cs),pnumber(pn){}
    Page(const Page& rhs):contents(rhs.contents),pnumber(rhs.pnumber){}
    virtual ~Page(){contents.clear();}
    virtual void print_output(){return;}
    virtual void add_choice(int dest_number, std::string content){throw WrongVirtualError(); }
};

class ChoosePage:public Page{
    private:
        std::vector<int> pageNumbers;
        std::vector<std::string> choice_contents;
    public:
        ChoosePage():Page(), pageNumbers(),choice_contents(){}
        ChoosePage(std::vector<std::string> cs,int pn):Page(cs,pn), pageNumbers(),choice_contents(){}
        ChoosePage(const ChoosePage& rhs): Page(rhs), pageNumbers(rhs.pageNumbers),choice_contents(rhs.choice_contents){}//这里的基类初始化疑惑
        virtual void add_choice(int dest_number, std::string content){
            pageNumbers.push_back(dest_number);
            choice_contents.push_back(content);
        } 
        virtual void print_output(){
            for(size_t i=0;i<Page::contents.size();i++){
                std::cout<<Page::contents[i]<<std::endl;
            }
            std::cout<<std::endl;
            std::cout<<"What would you like to do?"<<std::endl;
            std::cout<<std::endl;
            for(size_t i=0;i<choice_contents.size();i++){
                std::cout<<i+1<<". "<<choice_contents[i]<<std::endl;
            }
            return;
        }
    virtual ~ChoosePage(){}

};
class Story{
protected:
    std::string directory;
    std::vector<std::string> contents;
public:
    std::vector<Page*> pages;
    Story():directory(),contents(), pages(){}
    Story(std::string dicts):directory(dicts), contents(),pages(){
        directory = directory+"/";
        std::string story_dict = directory+"story.txt";
        contents = read_file(story_dict.c_str());
    }
    Story(const Story& rhs):directory(rhs.directory),contents(rhs.contents),pages(rhs.pages){
//implement deepcopy
    }
    virtual ~Story(){contents.clear();
//implement deep delete
    }
    void parse_choice(std::string line){
        int first_col=-1, second_col = -1;
        size_t tmp = -1;
        for(size_t i=0;i<line.length();i++){
            if(line[i]==':'){
                tmp=i;
                first_col = i;
                break;
            }
        }
        for(size_t i=tmp+1;i<line.length();i++){
            if(line[i]==':'){
                second_col = i;
                break;
            }
        }
        if(first_col==-1||second_col==-1){
            throw ContentError();
        }
        std::string pnumber = line.substr(0,first_col);
        std::string dnumber = line.substr(first_col+1, second_col-(first_col+1));
        std::string choice_content = line.substr(second_col+1,line.length()-second_col-1);
        int pn = std::atoi(pnumber.c_str());
        int dn = std::atoi(dnumber.c_str());
        bool matched=false;
        for(size_t i=0;i<pages.size();i++){
                Page* cur = pages[i];
            if(cur->pnumber==pn){
                cur->add_choice(dn, choice_content);
                matched=true;
                break;
            }
        }
        if(matched==false){
            throw ContentError();
        }
        return;
    }
    void parse_page(std::string line, int at_idx){
        int col_idx=-1;
        for(size_t i=0;i<line.length();i++){
            if(line[i]==':'){
                col_idx=i;
                break; }
        }
        if(col_idx==-1){throw ContentError();}
        std::string pnumber = line.substr(0,at_idx);
        std::string ptype = line.substr(at_idx+1, col_idx-(at_idx+1));
        std::string filename = line.substr(col_idx+1, line.length()-col_idx-1);
        int pn = std::atoi(pnumber.c_str());
        if(ptype[0]=='N'){
            filename = directory+filename;
            std::vector<std::string> contents = read_file(filename.c_str());
            Page* page = new ChoosePage(contents,pn);
            pages.push_back(page);
        }else{
        std::cout<<pages.size()<<std::endl;
        }
        return;
    }
    void parse_line(std::string line){
    int at_idx =-1;
    for(size_t i=0;i<line.length();i++){
        if(line[i]=='@'){
            at_idx = i;
            break;
        }
    }
    if(at_idx==-1){
        parse_choice(line);
    }else{
        parse_page(line,at_idx);
    }
    return;
    }
    bool is_blank_line(std::string s){
    if(s.length()==0){return true;}
    for(size_t i=0;i<s.length();i++){
        if(std::isspace(s[i])==0){
            return false;
        }
    }
    return true;
    }
    void parse_each_line(){
        for(size_t i=0;i<contents.size();i++){
            std::cout<<contents[i]<<std::endl;
            if(!is_blank_line(contents[i])){
            parse_line(contents[i]);
            }
        }
    std::cout<<"finish printing!"<<std::endl;
        return;
    };
    void print_pages(){
    std::cout<<"start printing!"<<std::endl;
    for(size_t i=0;i<pages.size();i++){
        pages[i]->print_output();
        }
    return;
    }
};
