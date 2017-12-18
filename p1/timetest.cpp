#include <iostream>
#include <string>
#include <fstream>
#include "CPUTimer.h"
#include "LinkedList.h"
#include "CursorList.h"
#include "StackAr.h"
#include "StackLi.h"
#include "QueueAr.h"
#include "SkipList.h"


using namespace std;

vector< CursorNode<int> > cursorSpace(2500001);


int getChoice();
void RunList(char* filename);
void RunCursorList(char* filename);
void RunStackAr(char* filename);
void RunStackLi(char* filename);
void RunQueueAr(char* filename);
void RunSkipList(char* filename);


int main()
{
  char filename[80];
  CPUTimer ct;
  int choice;

  cout << "Filename >> ";
  cin >> filename;

  do
  {
    choice = getChoice();
    ct.reset();
    
    switch(choice)
    {
      case 1: RunList(filename); break;
      case 2: RunCursorList(filename); break;
      case 3: RunStackAr(filename); break;
      case 4: RunStackLi(filename); break;
      case 5: RunQueueAr(filename); break;
      case 6: RunSkipList(filename); break;
    }

    cout << "CPU time: " << ct.cur_CPUTime() << endl;

  } while(choice > 0);

  return 0;
}


int getChoice()
{
  int opt;

  cout << "\n      ADT Menu\n"
  << "0. Quit\n1. LinkedList\n2. CursorList\n3. StackAr\n"
  << "4. StackLi\n5. QueueAr\n6. SkipList\n"
  << "Your choice >> ";
  cin >> opt;
  return opt;
}


void RunList(char* filename)
{
  ifstream inf(filename);
  List<int> list;
  ListItr<int> itr = list.zeroth();
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.insert(num, itr);
    else
      list.remove(num);
  }

  inf.close();
}


void RunCursorList(char* filename)
{
  ifstream inf(filename);

  CursorList<int> list(cursorSpace);
  CursorListItr<int> itr = list.zeroth();
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.insert(num, itr);
    else
      list.remove(num);
  }

  inf.close();  
}


void RunStackAr(char* filename)
{
  ifstream inf(filename);

  StackAr<int> list(2500001);
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.push(num);
    else
      list.pop();
  }

  inf.close();  
}


void RunStackLi(char* filename)
{
  ifstream inf(filename);

  StackLi<int> list;
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.push(num);
    else
      list.pop();
  }

  inf.close();  
}


void RunQueueAr(char* filename)
{
  ifstream inf(filename);

  Queue<int> list(2500001);
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.enqueue(num);
    else
      list.dequeue();
  }

  inf.close();  
}


void RunSkipList(char* filename)
{
  ifstream inf(filename);

  SkipList<int> list(0, 2500001);
  char temp[200];
  char option;
  int num;

  inf.getline(temp, 200);

  while (inf >> option >> num)
  {
    if (option == 'i')
      list.insert(num);
    else
      list.deleteNode(num);
  }

  inf.close();
}

