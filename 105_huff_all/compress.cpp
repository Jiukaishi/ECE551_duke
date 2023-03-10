#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <map>
#include <queue>
#include <assert.h>
#include <functional>
#include <stdlib.h>
#include "readFreq.h"
#include "node.h"
#include <fstream>


void writeHeader(BitFileWriter * bfw, const std::map<unsigned,BitString> &theMap) {
  for (int i =0 ; i < 257; i++) {
    std::map<unsigned,BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      bfw->writeByte(it->second.size());
      bfw->writeBitString(it->second);
    }
    else {
      bfw->writeByte(0);
    }
  }
}

void writeCompressedOutput(const char* inFile,
			   const char *outFile,
			   const std::map<unsigned,BitString> &theMap ){
  BitFileWriter bfw(outFile);
  writeHeader(&bfw,theMap);

  //WRITE YOUR CODE HERE!
  //open the input file for reading
    std::ifstream f(inFile);
      char ch;
        while (f.get(ch)) {
            if(theMap.find((unsigned char)ch)!=theMap.end()){
                bfw.writeBitString((theMap.find((unsigned char) ch))->second);
            }else{
                throw "map error\n";
            }
        }
        if(theMap.find(256)!=theMap.end()){
            bfw.writeBitString((theMap.find(256))->second);
        }else{
        throw "map error\n";
        }
    
        //        it = theMap.find((unsigned char) ch);
        //            assert(it != theMap.end());
        //                bfw.writeBitString(it->second);
        //                  }
        //
        //                    //dont forget to lookup 256 for the EOF marker, and write it out.
        //                      it = theMap.find(256);
        //                        assert(it != theMap.end());
        //                          bfw.writeBitString(it->second);
  //You need to read the input file, lookup the characters in the map,
  //and write the proper bit string with the BitFileWriter

  //dont forget to lookup 256 for the EOF marker, and write it out.

  //BitFileWriter will close the output file in its destructor
  //but you probably need to close your input file.
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr,"Usage: compress input output\n");
    return EXIT_FAILURE;
  }
  //WRITE YOUR CODE HERE
  //Implement main
  //hint 1: most of the work is already done. 
  //hint 2: you can look at the main from the previous tester for 90% of this

  const char *f = argv[1], *outFile = argv[2];
  uint64_t *counts = readFrequencies(f);
  Node *hftree = buildTree(counts);
  delete[] counts;
  BitString bs; // ouch..
std::map<unsigned, BitString> theMap;
   hftree->buildMap(bs, theMap);
    delete hftree;
writeCompressedOutput(f, outFile, theMap);
  return EXIT_SUCCESS;
}
