#include "StackLi.h"
#include "LinkedList.h"

using namespace std;

class LongInt
{
  public:
    List<int> list;
    friend istream& operator>>(istream &is, LongInt &li);
    friend ostream& operator<<(ostream &os, const LongInt &li);
    LongInt operator+(const LongInt &right);
    int addRemainingNums(ListItr<int>& itr, ListItr<int>& sumitr);
};