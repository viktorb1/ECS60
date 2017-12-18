#include "LongInt.h"


istream& operator>>(istream& is, LongInt& li)
{
  ListItr<int> itr = li.list.zeroth();

  while(isdigit(is.peek()))
  {
    int n = is.get() - 48; // correction for ascii
    li.list.insert(n, itr);
  } // while next character is a digit

  cin.ignore(1); // ignore newline character
  return is;
}


ostream& operator<<(ostream& os, const LongInt& li)
{
  ListItr<int> itr = li.list.first();
  StackLi<int> temp;

  while(!itr.isPastEnd())
  {
    temp.push(itr.retrieve()); // add number to stack
    itr.advance(); 
  }

  while(!temp.isEmpty())
    os << temp.topAndPop(); // print and delete stack

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

    if (leftitr.retrieve() + rightitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;

    sumitr.advance();
    leftitr.advance();
    rightitr.advance();
  }

  while(!leftitr.isPastEnd())
  {
    sum.list.insert(leftitr.retrieve() + carry, sumitr);

    if (leftitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;

    sumitr.advance();
    leftitr.advance();
  }

  while(!rightitr.isPastEnd())
  {
    sum.list.insert(rightitr.retrieve() + carry, sumitr);

    if (rightitr.retrieve() + carry >= 10)
      carry = 1;
    else
      carry = 0;
    
    sumitr.advance();
    rightitr.advance();
  }

  if (carry == 1)
    sum.list.insert(1, sumitr);

  return sum;
}