#include "headers.h"

#define SIZE 100

Process circ_que[SIZE];
int front = -1, rear = -1;

// Check if the queue is full
int isFull() {
  if ((front == rear + 1) || (front == 0 && rear == SIZE - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int isEmptyQueue() {
  if (front == -1) return 1;
  return 0;
}

// Adding an process
int enQueue(Process * process) {
  if (isFull())
    return -1;
  else {
    if (front == -1) front = 0;
    rear = (rear + 1) % SIZE;
    circ_que[rear] = *process;
    return 0;
  }
}

// Removing an element
Process deQueue() {
  Process element;
  if (isEmptyQueue()) {
    //printf("\n Queue is empty !! \n");
    return (-1);
  } else {
    element = circ_que[front];
    if (front == rear) {
      front = -1;
      rear = -1;
    } 
    // Q has only one element, so we reset the 
    // queue after dequeing it. ?
    else {
      front = (front + 1) % SIZE;
    }
    //printf("\n Deleted element is the one with id -> %d \n", element.id);
    return element
  }
}

// Display the queue
void display() {
  int i;
  if (isEmptyQueue())
    printf(" \n Empty Queue\n");
  else {
    printf("\n Front -> %d ", front);
    printf("\n Items -> ");
    for (i = front; i != rear; i = (i + 1) % SIZE) {
      printf("%d ", circ_que[i].id);
    }
    printf("%d ", circ_que[i].id);
    printf("\n Rear -> %d \n", rear);
  }
}
