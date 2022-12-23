#include "headers.h"


#define SIZE 100


struct circQueue
{
  int front;
  int rear;
  Process circ_que[SIZE];
};
typedef struct circQueue circQueue;

circQueue* CreateQueueM(){
  circQueue* newQueue = (circQueue*)malloc(sizeof(circQueue));
  newQueue->front=-1;
  newQueue->rear=-1;
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
void enQueueM(circQueue* Queue, Process* process) {
  if (isFullM(Queue))
    printf("\n Queue is full!! \n");
  else {
    if (Queue->front == -1) Queue->front = 0;
    Queue->rear = (Queue->rear + 1) % SIZE;
    Queue->circ_que[Queue->rear] = process;
    printf("\n Inserted -> %d \n", process);
  }
}

// Removing an element
int deQueueM(circQueue* Queue) {
  Process element;
  if (isEmptyQueueM(Queue)) {
    printf("\n Queue is empty !! \n");
    return (-1);
  } else {
    element = Queue->circ_que[Queue->front];
    if (Queue->front == Queue->rear) {
      Queue->front = -1;
      Queue->rear = -1;
    }
    // Q has only one element, so we reset the
    // queue after dequeing it. ?
    else {
      Queue->front = (Queue->front + 1) % SIZE;
    }
    printf("\n Deleted element is the one with id -> %d \n", element.id);
    return (element.id);
  }
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