#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()


int LeafNode::getMinimum() const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  if (count < leafSize)
    insertNormal(value);
  else
  {
    makeValueMax(value);

    if (leftSibling && leftSibling->getCount() < leafSize)
      insertLeft(value);
    else if (rightSibling && rightSibling->getCount() < leafSize)
      insertRight(value);
    else
      return splitRight(value);
  }

  return NULL;
}  // LeafNode::insert()


void LeafNode::insertNormal(int value)
{
  int i = count - 1;

  while (i >= 0 && value < values[i])
  {
    values[i+1] = values[i];
    i--;
  }

  values[i+1] = value;
  count++;


  if(parent)
    parent->updateMinimum();
}


void LeafNode::makeValueMax(int& value)
{
  int max = value;
  int i = count - 1;

  if (value <  values[i])
  {
    max = values[i];
    count--;
    insertNormal(value);
  }

  value = max;
}

void LeafNode::insertLeft(int value) 
{
  static_cast<LeafNode*>(leftSibling)->insertNormal(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i+1];

  values[count-1] = value;

  if(parent)
    parent->updateMinimum();
}

void LeafNode::insertRight(int value)
{
  static_cast<LeafNode*>(rightSibling)->insertNormal(value);
}


LeafNode* LeafNode::splitRight(int value)
{
  LeafNode* newRightNode = new LeafNode(leafSize, parent, this, rightSibling);

  if (rightSibling)
    rightSibling->setLeftSibling(newRightNode);

  rightSibling = newRightNode;

  int i = (leafSize + 1) / 2, j = 0;

  while (i < leafSize)
    newRightNode->values[j++] = values[i++];

  newRightNode->values[j++] = value; // value contains maximum
  newRightNode->count = j;
  count = (leafSize + 1) / 2;

  if(parent)
    parent->updateMinimum();

  return newRightNode;
}

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


