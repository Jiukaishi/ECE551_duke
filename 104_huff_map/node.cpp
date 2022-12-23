#include "node.h"
#include "assert.h"
void Node::buildMap(BitString b, std::map<unsigned,BitString> & theMap) {
    if(sym==NO_SYM){
    assert(left!=NULL && right!=NULL);
    BitString l = b.plusZero();
    left->buildMap(l, theMap);
    BitString r = b.plusOne();
    right->buildMap(r, theMap);
    }else{
    assert(left==NULL&&right==NULL);
    theMap[sym] = b;
    return;
    }    
    return;
}

