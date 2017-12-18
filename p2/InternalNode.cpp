#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum() const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  int i = count - 1; // i represents

  while(keys[i] > value && i > 0)
    i--;

  BTreeNode* splitnode = children[i]->insert(value);
  // splitnode = new node that was created after split

  if (splitnode && count < internalSize) // parent node is not full
    insertNormal(splitnode);
  else if (splitnode)
  {
    BTreeNode* max = makeChildMax(splitnode);

    if (leftSibling && leftSibling->getCount() < internalSize)
      insertLeft(max);
    else if (rightSibling && rightSibling->getCount() < internalSize)
      insertRight(max);
    else
      return splitRight(max);
  }

  return NULL; // to avoid warnings for now.
} // InternalNode::insert()


InternalNode* InternalNode::splitRight(BTreeNode* max)
{
  InternalNode* newRightNode = new InternalNode(internalSize, leafSize, parent, this, rightSibling);

  if (rightSibling)
    rightSibling->setLeftSibling(newRightNode);

  rightSibling = newRightNode;

  int i = (internalSize + 1) / 2, j = 0;

  while (i < internalSize)
  {
    newRightNode->children[j] = children[i];
    newRightNode->keys[j] = keys[i];
    children[i]->setParent(newRightNode);
    j++, i++;
  }

  newRightNode->children[j] = max; // value contains maximum
  newRightNode->keys[j] = max->getMinimum();
  max->setParent(newRightNode);
  j++;
  newRightNode->count = j;
  count = (internalSize + 1) / 2;

  if(parent)
    parent->updateMinimum();

  return newRightNode;
}


void InternalNode::insertRight(BTreeNode* node)
{
  static_cast<InternalNode*>(rightSibling)->insertNormal(node);
}

void InternalNode::insertLeft(BTreeNode* max)
{
  static_cast<InternalNode*>(leftSibling)->insertNormal(children[0]);

  for(int i = 0; i < count - 1; i++)
  {
    keys[i] = keys[i+1];
    children[i] = children[i+1];
  }

  children[count-1] = max;
  keys[count-1] = max->getMinimum();
  max->setParent(this);


  if(parent)
    parent->updateMinimum();
}


BTreeNode* InternalNode::makeChildMax(BTreeNode* unattachedChild)
{
  BTreeNode* max = unattachedChild;
  int i = count - 1;

  if (unattachedChild->getMinimum() < children[i]->getMinimum())
  {
    max = children[i];
    keys[i] = unattachedChild->getMinimum();
    count--;
    insertNormal(unattachedChild);
  }

  return max;
}


void InternalNode::insertNormal(BTreeNode* child)
{
  int i = count - 1; // i represents

  while(i >= 0 && keys[i] > child->getMinimum())
  {
    children[i+1] = children[i];
    keys[i+1] = keys[i];
    i--;
  }

  children[i+1] = child; //sets equal to the child
  keys[i+1] = child->getMinimum(); // adds minimum
  child->setParent(this); // updates child's parent
  count++;

  if(parent)
    parent->updateMinimum();
}


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  insertNormal(oldRoot);
  insertNormal(node2);

  oldRoot->setLeftSibling(NULL);
  oldRoot->setRightSibling(node2);
  node2->setLeftSibling(oldRoot);
  node2->setRightSibling(NULL);
  // students must write this
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // students may write this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


void InternalNode::updateMinimum()
{
  for (int i = 0; i < count; i++)
    keys[i] = children[i]->getMinimum();

  if (parent)
    parent->updateMinimum();
}