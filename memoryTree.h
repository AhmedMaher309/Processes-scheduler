#include "headers.h"


//Each block is a power of 2
MemoryBlock* root;


void createTree(MemoryBlock* memBlock, int n)
{
    if (n==0)  return;
    memBlock->left=(MemoryBlock*)malloc(sizeof(MemoryBlock));
    memBlock->right=(MemoryBlock*)malloc(sizeof(MemoryBlock));
    memBlock->left->blockSize=memBlock->right->blockSize=n;
    memBlock->left->allocated=memBlock->right->allocated=0;
    createTree(memBlock->left, memBlock->blockSize/2);
    createTree(memBlock->right, memBlock->blockSize/2);
}

void initializeMemory()
{
    root->blockSize=1024;
    root->left=root->right=NULL;
    root->allocated=0;
   // createTree(root,1024);
}

MemoryBlock* returnTheMemory(MemoryBlock* head, int allocateSize)
{
    //base case
    if (head->blockSize==allocateSize && head->allocated==0)
    {
        return head;
    }

    if (head->left==NULL)
    {
        head->left=(MemoryBlock*)malloc(sizeof(MemoryBlock));
        head->left->blockSize=head->(blockSize/2);
        head=head->left;
        head->left=head->right=NULL;
        return returnTheMemory(head,allocateSize);    
    }
    else if(head->right==NULL)
    {
        head->right=(MemoryBlock*)malloc(sizeof(MemoryBlock));
        head->right->blockSize=head->(blockSize/2);
        head=head->right;  
        head->left=head->right=NULL;
        return returnTheMemory(head,allocateSize);
    }
    else 
        return NULL;

}

void allocateMemory(Process* process)
{
    int allocateSize=1;
    //round up to next power of 2
    while (allocateSize < process->memSize)
        allocateSize=allocateSize*2;
    
    MemoryBlock*toBeAllocated=returnTheMemory(root,allocateSize);
   
    //TO DO:
    //Find the nearest power of 2 that is free 
    //Sets its status as allocated
    //Set process->allocatedMem to be able to find it and free it again
}


void deallocateMemory(Process* process)
{

}

