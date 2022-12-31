#include "headers.h"


//Each block is a power of 2
MemoryBlock* root;
root->blockSize=1024;
root->left=(MemoryBlock*)malloc(sizeof(MemoryBlock));
root->right=(MemoryBlock*)malloc(sizeof(MemoryBlock));

void createTree(MemoryBlock* memBlock, int n)
{
    if (n==0)  return;

    memBlock->left=(MemoryBlock*)malloc(sizeof(MemoryBlock));
    memBlock->right=(MemoryBlock*)malloc(sizeof(MemoryBlock));
    memBlock->left->blockSize=memBlock->right->blockSize=n;

    createTree(memBlock->left, memBlock->blockSize/2);
    createTree(memBlock->right, memBlock->blockSize/2);

}


void initializeMemory()
{
    createTree(root,1024);
}


void allocateMemory(Process* process)
{
    int allocateSize=1;
    //round up to next power of 2
    while (allocateSize < process->memSize)
        allocateSize=allocateSize*2;
    
    //TO DO:
    //Find the nearest power of 2 that is free 
    //Sets its status as allocated
    //Set process->allocatedMem to be able to find it and free it again
}

