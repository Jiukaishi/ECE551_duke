#include <exception>

#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V> {
 protected:
  struct Node {
    K key;
    V value;
    Node * left;
    Node * right;
    Node(K key, V value) : key(key), value(value), left(NULL), right(NULL) {}
  };
  Node * root;

 public:
  BstMap() : root(NULL) {}
  virtual void add(const K & key, const V & value) {
    Node ** cur = const_cast<Node **> (&root);
    while (*cur != NULL) {
      if ((*cur)->key == key) {
        ((*cur)->value) = value;
        return;
      }
      else if ((*cur)->key < key) {
        cur = &((*cur)->right);
      }
      else {
        cur = &((*cur)->left);
      }
    }
    *cur = new Node(key, value);
    return;
  }
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node ** cur =  const_cast<Node **> (&root);
    while (*cur != NULL) {
      if (((*cur)->key) == key) {
break;
      }
      else if ((*cur)->key < key)
        cur = &((*cur)->right);
      else
        cur = &((*cur)->left);
    }
   if(*cur==NULL){ throw std::invalid_argument("key not found");}
   return (*cur)->value;
  }
  virtual void remove(const K & key) {
    Node ** cur =const_cast<Node **> (&root);
    lookup(key);
    while (cur != NULL) {
      if ((*cur)->key == key) {
        if ((*cur)->left == NULL) {
          Node * tmp = *cur;
          *cur = (*cur)->right;
          delete tmp;
        }
        else if ((*cur)->right == NULL) {
          Node * tmp = *cur;
          *cur = (*cur)->left;
          delete tmp;
        }
        else {
         Node * replaced = rightMost((*cur)->left);
          K k = replaced->key;
          V v = replaced->value;
          Node *tmp = *cur;
          remove(replaced->key);
        *cur = new Node(k, v);
          (*cur)->left = tmp->left;
          (*cur)->right = tmp->right;
          delete tmp;
    //      Node * tmp = *cur;

      //    *cur = new Node(k, v);
        //  (*cur)->left = tmp->left;
          //(*cur)->right = tmp->right;
         // delete tmp;
        }
        return;
      }
      else if ((*cur)->key < key) {
        cur = &((*cur)->right);
      }
      else {
        cur = &((*cur)->left);
      }
    }
    return;
  }
  Node * rightMost(Node * r) {
    Node * res = r;
    while (res->right != NULL) {
      res = res->right;
    }
    return res;
  }
  void freeTree(Node* root){
      if(root==NULL){return;}
      freeTree(root->left);
      freeTree(root->right);
      delete root;
      return;
  }
 virtual ~BstMap(){freeTree(root);}

};
