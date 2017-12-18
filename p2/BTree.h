#ifndef BTREE_H
  #define BTREE_H

#include "BTreeNode.h"

class BTree
{
  BTreeNode *root;
  int internalSize; // M
  int leafSize; // L
public:
  BTree(int ISize, int LSize);
  void insert(int value);
  void print();
}; // BTree class
#endif

