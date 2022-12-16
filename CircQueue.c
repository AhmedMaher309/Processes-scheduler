//#include "headers.h"

#define SIZE 4

struct Process circ_que[SIZE];
int front = -1, rear = -1;

// Check if the queue is full
int isFull() {
  if ((front == rear + 1) || (front == 0 && rear == SIZE - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int isEmpty() {
  if (front == -1) return 1;
  return 0;
}

// Adding an process
void enQueue(struct Process * process) {
  if (isFull())
    printf("\n Queue is full!! \n");
  else {
    if (front == -1) front = 0;
    rear = (rear + 1) % SIZE;
    circ_que[rear] = *process;
    printf("\n Inserted -> %d \n", process->id);
  }
}

// Removing an element
int deQueue() {
  struct Process element;
  if (isEmpty()) {
    printf("\n Queue is empty !! \n");
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
    printf("\n Deleted element is the one with id -> %d \n", element.id);
    return (element.id);
  }
}

// Display the queue
void display() {
  int i;
  if (isEmpty())
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

int main() {
  // Fails because front = -1
  deQueue();
  
  int c = 0;
  struct Process p;
  int n;
  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  struct Process *p_ptr = &p;
  c++;
  enQueue(p_ptr);
  display();

  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);
  display();

  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);
  display();

  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);

  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);
  display();
  
  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);
  display();

  scanf("%d",&n);
  p.id = n;
  p.Priority = c;
  p.RunTime = 1;
  p.ArrivalTime = 4;
  c++;
  enQueue(p_ptr);
  display();


  display();
  deQueue();

  display();


  return 0;
}