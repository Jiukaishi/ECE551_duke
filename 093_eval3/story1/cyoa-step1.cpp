#include <iostream>
#include "page.h"
#include <cstdlib>
int main(int argc, char** argv){
if(argc!=2){
    std::cerr<<"check input arg!\n";
     exit(EXIT_FAILURE);
}
Story story(argv[1]);
story.parse_each_line();
story.print_pages();
return EXIT_SUCCESS;

}
