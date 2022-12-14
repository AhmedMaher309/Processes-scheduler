
#include "headers.h"
#define MAX 100

struct Process pri_que[MAX];
int front, rear;

typedef enum
{
    runTime = 1,
    priority = 2
} Key;

Key currentKey;

void setKey(Key k)
{
    currentKey = k;
}

// Function to create an empty priority queue
void create()
{
    front = rear = -1;
}

// Function to check priority and place element

void checkPriority(struct Process *process)
{
    int i, j;

    if (currentKey == priority)
    {
        for (i = front; i <= rear; i++)
        {
            if (process->priority <= pri_que[i].priority)
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
    else
    {
        for (i = front; i <= rear; i++)
        {
            if (process->runTime <= pri_que[i].runTime)
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
}

// Function to insert value into priority queue
// assuming low value is the high prioriity
void insert_by_priority(struct Process *process)
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
        checkPriority(process);
    rear++;
}

// Function to delete an element from queue
void delete_by_priority(int id)
{
    int i;
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty no elements to delete \n");
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
            pri_que[i].priority = -99;
            rear--;

            if (rear == -1)
                front = -1;
            return;
        }
    }
    printf("\n%d not found in queue to delete", id);
}

// function to pop first element
Process popQueue()
{
    Process temp = pri_que[front];
    delete_by_priority(pri_que[front].id);
    return temp;
}

// Function to display queue elements
void display_pqueue()
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty \n");
        return;
    }

    for (; front <= rear; front++)
    {
        printf(" %d ", pri_que[front].id);
    }
    printf("\n");
    front = 0;
}

int isEmpty()
{
    int status;
    if (front == -1 && rear == -1)
        status = 1;
    else
        status = 0;

    return status;
}