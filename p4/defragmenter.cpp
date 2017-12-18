#include "defragmenter.h"
#include "DefragRunner.h"

Defragmenter::Defragmenter(DiskDrive *diskDrive)
{
  int numFiles = diskDrive->getNumFiles();
  int index = 2, nextBlockID = -1;
  const int arrSize = 500001;
  int *newIDs = new int[arrSize];
  int tempSize = 11276, tempCount = 0;
  DiskBlock* *temp = new DiskBlock*[tempSize];
  int *tempIndex = new int[tempSize];
  //9396
  for(int i = 0; i < arrSize; i++)
  {
    newIDs[i] = i;
  }

  for(int i = 0; i < tempSize; i++)
  {
    tempIndex[i] = -1;
    temp[i] = NULL;
  }

  int maxEmptyIndex = diskDrive->getCapacity() - 1;

  for(int i = maxEmptyIndex; i > 1; i--) 
  {
    if (diskDrive->FAT[i] == false)
    {
      maxEmptyIndex = i;
      break;
    }
  }


  for (int i = 0; i < numFiles; i++)
  {
    DiskBlock *currBlock = NULL, *swapBlock = NULL;
    int blockID = diskDrive->directory[i].getFirstBlockID();
    int numDiskBlocksInFile = diskDrive->directory[i].getSize();
    diskDrive->directory[i].setFirstBlockID(index);

    for (int j = 0; j < numDiskBlocksInFile; j++)
    {
      while (blockID != newIDs[blockID])
        blockID = newIDs[blockID];

      bool found = false;

      int low = 0;
      int high = tempCount - 1;
      int middle;
            
      while(high >= low) {
        middle = (low + high) / 2;
        if (tempIndex[middle] == blockID)
        {
          currBlock = temp[middle];
          found = true;
          break;
        }
            if(tempIndex[middle] < blockID)
                 low = middle + 1;

            if(tempIndex[middle] > blockID)
                high = middle - 1;
       }


      if (!found)
      {
        currBlock = diskDrive->readDiskBlock(blockID);
      }

      nextBlockID = currBlock->getNext();// perform lookup

      if (nextBlockID != 1)
        currBlock->setNext(index + 1);

      if (blockID != index) // update disk
      {
        if (diskDrive->FAT[index] == true)
        {
          if (tempCount < tempSize)
          {
            swapBlock = diskDrive->readDiskBlock(index);
            temp[tempCount] = swapBlock;
            tempIndex[tempCount] = index;
            tempCount++;
          }
          else
          {
            swapBlock = diskDrive->readDiskBlock(index);
            diskDrive->writeDiskBlock(swapBlock, maxEmptyIndex);
            newIDs[index] = maxEmptyIndex;
            diskDrive->FAT[maxEmptyIndex] = true;
            if (blockID > maxEmptyIndex)
              maxEmptyIndex = blockID;
            else
            {
              for(int i = maxEmptyIndex; i > 1; i--) 
              {
                if (diskDrive->FAT[i] == false)
                {
                  maxEmptyIndex = i;
                  break;
                }
              }
            }

            delete swapBlock;
            swapBlock = NULL;
          }
        }

        diskDrive->writeDiskBlock(currBlock, index);
        diskDrive->FAT[index] = true;
        diskDrive->FAT[blockID] = false;
      }

      if (found)
      {
        for(int i = middle; i < tempCount-1; i++)
        {
          tempIndex[i] = tempIndex[i+1];
          temp[i] = temp[i+1];
        }
        tempCount--;
      }


      delete currBlock;
      currBlock = NULL;
      blockID = nextBlockID;
      index++;
    }
  }

  // delete maxHeap;
  delete[] newIDs;
  delete temp;
  // diskDrive->print();

} // Defragmenter()