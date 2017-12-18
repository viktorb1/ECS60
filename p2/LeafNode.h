#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode : public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);
  void insertNormal(int value);
  void makeValueMax(int& value);
  void insertLeft(int value);
  void insertRight(int value);
  LeafNode* splitRight(int value);
}; //LeafNode class

#endif
