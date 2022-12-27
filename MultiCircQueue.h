#include "headers.h"


#define SIZE 100


struct circQueue
{
  int front;
  int rear;
  int size;
  Process circ_que[SIZE];
};
typedef struct circQueue circQueue;

circQueue* CreateQueueM(){
  circQueue* newQueue = (circQueue*)malloc(sizeof(circQueue));
  newQueue->front =-1;
  newQueue->rear = -1;
  newQueue->size = 0;
  return newQueue;
}

// Check if the queue is full
int isFullM(circQueue* Queue) {
  if ((Queue->front == Queue->rear + 1) || (Queue->front == 0 && Queue->rear == SIZE - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int isEmptyQueueM(circQueue* Queue) {
  if (Queue->front == -1) return 1;
  return 0;
}

// Adding an process
int enQueueM(circQueue* Queue, Process* process) {
  if (isFullM(Queue))
    return -1;
  else {
    if (Queue->front == -1) Queue->front = 0;
    Queue->rear = (Queue->rear + 1) % SIZE;
    Queue->circ_que[Queue->rear] = *process;
    Queue->size++;
    return 0;
  }
}

// Removing an element
Process deQueueM(circQueue* Queue) {
  Process element;
    element = Queue->circ_que[Queue->front];
    if (Queue->front == Queue->rear) {
      Queue->front = -1;
      Queue->rear = -1;
      Queue->size = 0;
    }
    // Q has only one element, so we reset the
    // queue after dequeing it. ?
    else {
      Queue->front = (Queue->front + 1) % SIZE;
      Queue->size--;
    }
    
    return element;
}

// Display the queue
void displayM(circQueue* Queue) {
  int i;
  if (isEmptyQueueM(Queue))
    printf(" \n Empty Queue\n");
  else {
    printf("\n Front -> %d ", Queue->front);
    printf("\n Items -> ");
    for (i = Queue->front; i != Queue->rear; i = (i + 1) % SIZE) {
      printf("%d ", Queue->circ_que[i].id);
    }
    printf("%d ", Queue->circ_que[i].id);
    printf("\n Rear -> %d \n", Queue->rear);
  }
}