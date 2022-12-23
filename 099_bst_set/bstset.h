#include <exception>

#include "set.h"

template<typename T>
class BstSet : public Set<T> {
 protected:
  struct Node {
    T key;
    Node * left;
    Node * right;
    Node(T key) : key(key), left(NULL), right(NULL) {}
  };
  Node * root;

 public:
  BstSet() : root(NULL) {}
  BstSet(const BstSet & m) : root(NULL) { copyTree(&root, m.root); }
  void copyTree(Node ** n, const Node * m) {
    if (m != NULL) {
      *n = new Node(m->key);
      copyTree(&(*n)->left, m->left);
      copyTree(&(*n)->right, m->right);
    }
  }
  const BstSet & operator=(const BstSet & m) {
    if (&m != this) {
      BstSet temp = m;
      Node * t = root;
      root = temp.root;
      temp.root = t;
    }
    return *this;
  }
  virtual void add(const T & key) {
    Node ** cur = const_cast<Node **>(&root);
    while (*cur != NULL) {
      if ((*cur)->key == key) {
        return;
      }
      else if ((*cur)->key < key) {
        cur = &((*cur)->right);
      }
      else {
        cur = &((*cur)->left);
      }
    }
    *cur = new Node(key);
    return;
  }
  virtual bool contains(const T & key) const {
    Node ** cur = const_cast<Node **>(&root);
    while (*cur != NULL) {
      if (((*cur)->key) == key) {
        return true;
      }
      else if ((*cur)->key < key)
        cur = &((*cur)->right);
      else
        cur = &((*cur)->left);
    }
    return false;
  }
  virtual void remove(const T & key) {
    Node ** cur = const_cast<Node **>(&root);
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
          T k = replaced->key;
          Node * tmp = *cur;
          remove(replaced->key);
          *cur = new Node(k);
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
  void freeTree(Node * root) {
    if (root == NULL) {
      return;
    }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
    return;
  }
  virtual ~BstSet() { freeTree(root); }
};
