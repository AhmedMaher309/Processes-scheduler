#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#define MAX 10

struct Process pri_que[MAX];
int front, rear;
    

// Function to create an empty priority queue 
void create()
{
    front = rear = -1;
}

    
// Function to insert value into priority queue 
// assuming high value is the high prioriity
void insert_by_priority(struct Process* process)
{
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        pri_que[rear] = *process;
        return;
    }    
    else
        check(process);
    rear++;
}

    
// Function to check priority and place element 
void check(struct Process *process)
{
    int i,j;
    for (i = 0; i <= rear; i++)
    {
        if (process->Priority >= pri_que[i].Priority)
        {
            for (j = rear + 1; j > i; j--)
            {
                pri_que[j] = pri_que[j - 1];
            }
            pri_que[i] = *process;
            return;
        }
    }
    pri_que[i] = *process;
}


// Function to delete an element from queue 
void delete_by_priority(int id)
{
    int i;
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (id == pri_que[i].id)
        {
            for (; i < rear; i++)
            {
                pri_que[i] = pri_que[i + 1];
            }
        pri_que[i].Priority = -99;
        rear--;

        if (rear == -1) 
            front = -1;
        return;
        }
    }
    printf("\n%d not found in queue to delete", id);
}


// Function to display queue elements 
void display_pqueue()
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        printf(" %d ", pri_que[front].Priority);
    }
    front = 0;
}


void main()
{

   int n, ch;

   printf("\n1 - Insert an element into queue");
   printf("\n2 - Delete an element from queue");
   printf("\n3 - Display queue elements");
   printf("\n4 - Exit");
   int c = 0;
   create();
   while (1)
   {
      printf("\nEnter your choice : ");    
      scanf("%d", &ch);
      switch (ch)
      {
      case 1: 
            printf("\nEnter value to be inserted : ");
            scanf("%d",&n);
            struct Process p;
            p.id = c;
            p.Priority = n;
            p.RunTime = 1;
            p.ArrivalTime = 4;
            struct Process *p_ptr = &p;
            insert_by_priority(p_ptr);
            c++;
            break;

      case 2:
            printf("\nEnter value to delete : ");
            scanf("%d",&n);
            delete_by_priority(n);
            break;

      case 3: 
            display_pqueue();
            break;

      case 4: 
            exit(0);

      default: 
            printf("\nChoice is incorrect, Enter a correct choice");
      }

   }

}

