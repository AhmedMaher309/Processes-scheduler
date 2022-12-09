#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//enum State = {started = 1, running = 2, ready = 3, blocked = 4};

typedef struct 
{
    int id;
    int arrivalTime;
    int runTime;
    int priority;
    int startingTime;
    int waitingTime;
}Process;

struct Process *createProcess(int id, int priority, int runTime, int arrivalTime)
{
    struct Process *p = (struct Process *)malloc(sizeof(struct Process));
    p->id = id;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->runTime = runTime;
    return p;
}
