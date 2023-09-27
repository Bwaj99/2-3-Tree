#include "Tree.h" 

#include <iostream>
using namespace std ;

Tree::Tree(){
  root = nullptr; //initialize root node
}

Tree::~Tree(){
  delete root ; 
}

void Tree::insert(const string& str) {
  if (!root) { //create a new node with the given string as the small string
    root = new Node();
    root->small = str;
    return;
  }

  Node* targetNode = root;
  while (targetNode->left) {// traverse down the tree to find the appropriate position for the new string
    targetNode = targetNode->twoString() ?
    (targetNode->small > str ? targetNode->left :
    (targetNode->large < str ? targetNode->right : targetNode->middle)) :
    (targetNode->small > str ? targetNode->left : targetNode->middle);
  }
  
  if (targetNode->twoString()) { //split it into three nodes and insert the new string between
    string small, middle, large;
    if (targetNode->small > str) {
      small = str;
      middle = targetNode->small;
      large = targetNode->large;
    } 
    else {
      small = targetNode->small;
      if (targetNode->large < str) {
        middle = targetNode->large;
        large = str;
      } 
      else {
        middle = str;
        large = targetNode->large;
      }
    }
    
  Node* smallNode = new Node();
  smallNode->small = small;
  Node* largeNode = new Node();
  largeNode->small = large;
  targetNode->small = middle;
  targetNode->large = "";
  targetNode->left = smallNode;
  targetNode->middle = largeNode;
  targetNode->right = nullptr;
  smallNode->parent = targetNode;
  largeNode->parent = targetNode;
  shiftUp(targetNode);
    } 
    
  else {
    insertStringIntoNode(targetNode, str); //insert the new string into the node
    }
}

void Tree::insertStringIntoNode(Node* node, const string& str) {
  if (node->small > str) {
    node->large = node->small;
    node->small = str;
  } 
  else {
    node->large = str;
  }
}

void Tree::shiftUp(Node* node) {
  if (node->parent) {
    Node* parent = node->parent;
    if (parent->twoString()) {
      if (parent->small > node->small) {
        Node* newLarge = new Node();
        newLarge->left = parent->middle;
        newLarge->left->parent = newLarge;
        newLarge->middle = parent->right;
        newLarge->middle->parent = newLarge;
        newLarge->small = parent->large;
        parent->left = node;
        parent->middle = newLarge;
        parent->right = nullptr;
        parent->large = "";
        newLarge->parent = parent;
      } 
      else if (parent->large < node->small) {
        Node* newSmall = new Node();
        newSmall->left = parent->left;
        newSmall->left->parent = newSmall;
        newSmall->middle = parent->middle;
        newSmall->middle->parent = newSmall;
        newSmall->small = parent->small;
        parent->left = newSmall;
        parent->middle = node;
        parent->right = nullptr;
        parent->small = parent->large;
        parent->large = "";
        newSmall->parent = parent;
      } 
      else {
        Node* newSmall = new Node();
        newSmall->small = parent->small;
        newSmall->left = parent->left;
        newSmall->left->parent = newSmall;
        newSmall->middle = node->left;
        newSmall->middle->parent = newSmall;
        Node* newLarge = new Node();
        newLarge->small = parent->large;
        newLarge->left = node->middle;
        newLarge->left->parent = newLarge;
        newLarge->middle = parent->right;
        newLarge->middle->parent = newLarge;
        parent->left = newSmall;
        parent->middle = newLarge;
        parent->right = nullptr;
        parent->small = node->small;
        parent->large = "";
        newSmall->parent = parent;
        newLarge->parent = parent;
        delete node;
      }
      shiftUp(parent);
    } 
    else {
      if (parent->small > node->small) {
        parent->large = parent->small;
        parent->small = node->small;
        parent->left = node->left;
        parent->right = parent->middle;
        parent->middle = node->middle;
      } 
      else {
        parent->large = node->small;
        parent->middle = node->left;
        node->left->parent = parent;
        parent->right = node->middle;
        node->middle->parent = parent;
      }
      node->left->parent = parent;
      node->middle->parent = parent;
      delete node;
    }
  }
}

Node* Tree::findNode(const string& str) const {
  Node* searchNode = root;
  while (searchNode){
    if (searchNode->small == str || (searchNode->twoString() && searchNode->large == str)){
      return searchNode;
    }
    else {
      if (searchNode->small > str){
        searchNode = searchNode->left;
      }
      else{
        if (searchNode->twoString() && searchNode->large < str){
          searchNode = searchNode->right;
        }
        else{
          searchNode = searchNode->middle;
        }
      }
    }
  }
  return nullptr; //return null if string isn't found
}

// remove function
void Tree::remove(const string &str){
  Node* nodeToRemove = findNode(str); //located node for removal
  if (!nodeToRemove) {
    return;
  }
  if (nodeToRemove->twoString()){ //remove target string if node has two strings
    if (str == nodeToRemove->small){
      nodeToRemove->small = nodeToRemove->large;
      nodeToRemove->large = "";
    }
    else{
      nodeToRemove->large = "";
    }
  }
  else{ // node has only one string
    Node* parent = nodeToRemove->parent;
    if (parent) { //remove parent and update children
      if (parent->left == nodeToRemove) {
        parent->left = nullptr;
        parent->large = parent->middle->small;
        parent->middle = nullptr;
      } 
      else {
        parent->middle = nullptr;
        parent->large = parent->small;
        parent->small = parent->left->small;
        parent->left = nullptr;
      }
      
      delete nodeToRemove;
    }
        
    else { //root is node
      if (root->left) { //create a new root with its children
        Node* temp = new Node();
        temp->small = root->left->small;
        temp->large = root->middle->small;
        root = temp;
        delete nodeToRemove;
      } 
      else { //delete root if it's leaf
        delete root;
        root = nullptr;
      }
    }
  }
}

bool Tree::search(const string &key) const { //search helper function
  return searchHelper(root, key) ;
}

bool Tree::searchHelper(Node* currNode, const string &key) const {
  if (currNode) { 
    if (currNode -> small == key || currNode -> large == key){ //checks node to see if any values are same as key
      return true;
    }
    else { //recusively calls in direction and node
      if (key < currNode -> small) {
        return searchHelper(currNode -> left, key);
      }
      else if (key > currNode -> large) {
        return searchHelper(currNode -> right, key);
      }
      else {
        return searchHelper(currNode -> middle, key);
      }
    }
  }
  else {
    return false;
  }
}

void Tree::preOrder() const //traverses the tree in pre-order and prints out the values of each node
{
    preOrderHelper(root);
    cout << endl;
}

void Tree::inOrder() const //traverses the tree in in-order and prints out the values of each node
{
    inOrderHelper(root);
    cout << endl;    
}

void Tree::postOrder() const //traverses the tree in post-order and prints out the values of each node
{
    postOrderHelper(root);
    cout << endl;    
}

void Tree::preOrderHelper(Node* currNode) const //traverses the tree in pre-order and prints out the values of each node
{
  if (currNode){ //takes in current value as parameter
    if (currNode->twoString()){
      cout << currNode->small << ", ";
      preOrderHelper(currNode->left);
      cout << currNode->large << ", ";
      preOrderHelper(currNode->middle);
      preOrderHelper(currNode->right);
    }
    else{ //traverse left and middle subtrees
      cout << currNode->small << ", ";
      preOrderHelper(currNode->left);
      preOrderHelper(currNode->middle);
    }
  }
}

void Tree::inOrderHelper(Node* currNode) const {
    if (currNode){ // checking if the current node exists
      inOrderHelper(currNode->left);
      cout << currNode->small << ", ";
      inOrderHelper(currNode->middle);

      if (currNode->twoString()){
        cout << currNode->large << ", ";
        inOrderHelper(currNode->right);
      }
    }
  }  

void Tree::postOrderHelper(Node* currNode) const {
    if (currNode){ // checking if the current node exists
      if (currNode->twoString()){
            postOrderHelper(currNode->left);
            postOrderHelper(currNode->middle);
            cout << currNode->small << ", ";
            postOrderHelper(currNode->right);
            cout << currNode->large << ", ";
        }
      else{
        postOrderHelper(currNode->left);
        postOrderHelper(currNode->middle);
        cout << currNode->small << ", ";
      }
    }
  }  