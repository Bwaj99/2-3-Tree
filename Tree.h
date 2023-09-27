#ifndef TREE_H
#define TREE_H

#include "Node.h"
#include <string>

using namespace std;

class Tree {
public:
  Tree();
  ~Tree();
  void preOrder() const;
  void inOrder() const;
  void postOrder() const;
  void insert(const string&);
  void remove(const string&);
  bool search(const string&) const;
    
private:
  Node *root;
  Node* findNode(const string& ) const ;
  void insertStringIntoNode(Node* , const string& ); 
  void preOrderHelper(Node*) const;
  void inOrderHelper(Node*) const;
  void postOrderHelper(Node*) const;
  void splitNode(Node*, const string&) ;
  void shiftUp(Node* node) ;
  bool searchHelper(Node*, const string&) const;
};

#endif