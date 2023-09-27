#include "Node.h"

Node::Node() : left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}

bool Node::twoString() const{
  return !large.empty();
}

bool Node::twoChild() const {
  return left && middle && right;
}