#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// enum State = {started = 1, running = 2, ready = 3, blocked = 4};

// struct Process
// {
//     int id;
//     int arrivalTime;
//     int runTime;
//     int priority;
//     int startingTime;
//     int waitingTime;
//     int remainingTime;
// };
// typedef struct Process Process;

// Process *createProcess(int id, int priority, int runTime, int arrivalTime)
// {
//     Process *p = (Process *)malloc(sizeof(Process));
//     p->id = id;
//     p->priority = priority;
//     p->arrivalTime = arrivalTime;
//     p->runTime = runTime;
//     return p;
// }
