#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h> //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

typedef short bool;
#define true 1
#define false 0

#define SHKEY 300
#define QKEY 100
//====================================
typedef enum
{
    blocked = 1,
    started,
    running,
    finished,
} ProcessState;

enum Algorithm
{
    SJF = 1,
    HPF = 2,
    RR = 3,
    MLFL = 4
};

struct Process
{
    int id;
    int arrivalTime;
    int runTime;
    int priority;
    int startingTime;
    int waitingTime;
    //int realTime;
    int isBoosted;
    int remainingTime;
    int flagLast;
    ProcessState state;
    int forkId;
    int forkingID;
};
typedef struct Process Process;

Process *createProcess(int id, int priority, int runTime, int arrivalTime)
{
    Process *p = (Process *)malloc(sizeof(Process));
    p->id = id;
    p->priority = priority;
    p->arrivalTime = arrivalTime;
    p->runTime = runTime;
    p->waitingTime = 0;
    p->flagLast = 0;
    p->forkId = 0;
    p->remainingTime = runTime;
    p->forkingID = -1;
    p->isBoosted = 0;
    return p;
}

//====================================

///==============================
// don't mess with this variable//
int *shmaddr; //
//===============================
// struct for message queue used between process generator and scheduler
typedef struct
{
    long mtype;
    Process process;
} msgbuff;

int getClk()
{
    return *shmaddr;
}

/*
 * All processes call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
 */
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        // Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *)shmat(shmid, (void *)0, 0);
}

/*
 * All processes call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
 */

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}

// run takes fileName, compiles it, then runs the .out file
// argv1 and argv2 are the arguments needed to be passed to the .out file
void run(char *fileNameInput, char *argv1, char *argv2)
{
    char command[50];
    sprintf(command, "gcc %s.c -o %s.out", fileNameInput, fileNameInput);

    if (system(command) == -1) // this calls the bash window to execute the command
    {
        perror("failed to Compile .c file: ");
    }
    else
    {
        char path[20];
        sprintf(path, "%s.out", fileNameInput);
        char *absPath = realpath(path, NULL);
        if (execl(absPath, argv1, argv2, NULL) == -1)
        {
            perror("Error in execl: ");
        }
    }
}

int intMsgQueue(int key)
{
    int keyId = ftok("keyFile", key); // it's a best practice to use ftok to generate the key
    int msgQueueId = msgget(keyId, 0666 | IPC_CREAT);

    if (msgQueueId == -1)
    {
        perror("Error in creating message queue: ");
    }

    return msgQueueId;
}

void sendProcess(Process sProcess, int queueId)
{
    msgbuff msg;
    msg.process = sProcess;
    int sendStatus = msgsnd(queueId, &msg, sizeof(msg.process), !IPC_NOWAIT);

    if (sendStatus == -1)
    {
        perror("Error in sending in queue: ");
    }
}

Process recieveProcess(int queueId, int *state)
{
    msgbuff msg;

    *state = msgrcv(queueId, &msg, sizeof(msg.process), 0, IPC_NOWAIT);
    return msg.process;
}

#endif