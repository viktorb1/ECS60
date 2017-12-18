#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "BinaryHeap.h"

Defragmenter::Defragmenter(DiskDrive *diskDrive)
{
  int numFiles = diskDrive->getNumFiles();
  int index = 2, nextBlockID = -1;
  const int arrSize = 500000;
  int *newIDs = new int[arrSize];
  DiskBlock* *temp = new DiskBlock*[arrSize];
  int ramSize = 2305, ramCount = 0;

  for(int i = 0; i < arrSize; i++)
  {
    temp[i] = NULL;
    newIDs[i] = i;
  }

  int maxHeapSize = 200000, count = 0;
  BinaryHeap<int>* maxHeap = new BinaryHeap<int> (maxHeapSize);

  for(int i = diskDrive->getCapacity() - 1; i > 1; i--) 
  {
    if (diskDrive->FAT[i] == false)
    {
      maxHeap->insert(i);
      count++;
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

      if (temp[blockID])
      {
        currBlock = temp[blockID];
        ramCount--;
      }
      else
        currBlock = diskDrive->readDiskBlock(blockID);

      nextBlockID = currBlock->getNext();

      if (nextBlockID != 1)
        currBlock->setNext(index + 1);

      if (blockID != index) // update disk
      {
        if (diskDrive->FAT[index] == true)
        {
          if (ramCount < ramSize)
          {
            swapBlock = diskDrive->readDiskBlock(index);
            temp[index] = swapBlock;
            ramCount++;
          }
          else
          {
            int maxEmpty = maxHeap->findMax();
            swapBlock = diskDrive->readDiskBlock(index);
            diskDrive->writeDiskBlock(swapBlock, maxEmpty);
            newIDs[index] = maxEmpty;
            diskDrive->FAT[maxEmpty] = true;
            maxHeap->deleteMax();
            count--;
            delete swapBlock;
            swapBlock = NULL;
          }
        }

        diskDrive->writeDiskBlock(currBlock, index);
        diskDrive->FAT[index] = true;
        diskDrive->FAT[blockID] = false;
        
        if (count < maxHeapSize)
        {
          maxHeap->insert(blockID);
          count++;
        }
      }

      delete currBlock;
      currBlock = NULL;
      blockID = nextBlockID;
      index++;
    }
  }

  delete maxHeap;
  delete[] newIDs;
  delete[] temp;
} // Defragmenter()