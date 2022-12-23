#include "node.h"
#include <iostream>
Node * buildTree(uint64_t * counts) {
  priority_queue_t pq;
  for (size_t i=0; i<257; i++) {
    if (counts[i] > 0) {
      Node* cur = new Node(i, counts[i]);
      pq.push(cur);
    }
  }
  Node* cur=NULL;
  while (pq.empty()==false) {
    if(cur)
      pq.push(cur);
    Node * l = pq.top();
    pq.pop();
    Node * r = pq.top();
    pq.pop();

    cur = new Node(l, r);
  }
  return cur;
}

// vim: ts=2:sw=2:et
