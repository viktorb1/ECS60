#include "LongInt.h"


istream& operator>>(istream& is, LongInt& li)
{
  ListItr<int> itr = li.list.zeroth();

  while(isdigit(is.peek()))
  {
    int n = is.get() - 48;
    li.list.insert(n, itr);
  }

  cin.ignore(1);
  return is;
}


ostream& operator<<(ostream& os, const LongInt& li)
{
  ListItr<int> itr = li.list.first();
  StackLi<int> temp;

  while(!itr.isPastEnd())
  {
    temp.push(itr.retrieve());
    itr.advance(); 
  }

  while(!temp.isEmpty())
    os << temp.topAndPop();

  return os;
}


LongInt LongInt::operator+(const LongInt &right)
{
  LongInt sum;
  ListItr<int> leftitr = list.first();
  ListItr<int> rightitr = right.list.first();
  ListItr<int> sumitr = sum.list.zeroth();
  int carry = 0;

  while(!leftitr.isPastEnd() && !rightitr.isPastEnd())
  {
    int digit = (leftitr.retrieve() + rightitr.retrieve() + carry) % 10;
    sum.list.insert(digit, sumitr);
    sumitr.advance();

    if (leftitr.retrieve() + rightitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;

    leftitr.advance();
    rightitr.advance();
  }

  while(!leftitr.isPastEnd())
  {
    sum.list.insert(leftitr.retrieve() + carry, sumitr);
    sumitr.advance();

    if (leftitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;

    leftitr.advance();
  }

  while(!rightitr.isPastEnd())
  {
    sum.list.insert(rightitr.retrieve() + carry, sumitr);
    sumitr.advance();

    if (rightitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;

    rightitr.advance();
  }

  if (carry == 1)
    sum.list.insert(1, sumitr);

  return sum;
}