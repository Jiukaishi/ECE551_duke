#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include "readFreq.h"

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256 ) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
    std::ifstream f(fname);
    char c;
    uint64_t* array = new uint64_t[257];
    for(size_t i=0;i<257;i++){
        array[i]=0;
    }
    while(f.get(c)){
        array[(unsigned char)c]+=1;
    }
    f.close();
    array[256]++;
    return array;
}
