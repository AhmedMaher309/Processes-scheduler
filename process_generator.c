#include "headers.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int id;
    int arrivalTime;
    int runTime;
    int priority;

} Process;

void clearResources(int);

int getNumberOfLines(char* fileName)
{
    int count=0;
    FILE* filePtr=fopen(fileName, "r");
    char temp;
    temp= getc(filePtr);
    while (temp!=EOF)
        {
            if(temp=='\n')
                count++;
            temp=getc(filePtr);
        }
    fclose(filePtr);
    return count;
}

void readInputFiles(char* fileName, Process* processArray, int processCount)
{
    FILE* filePtr=fopen(fileName, "r");
    //Skip first line    
    fscanf(filePtr, "%*[^\n]\n");
    for (int i=0; i<processCount; i++)
    {
        fscanf(filePtr, "%d  %d  %d  %d", &(processArray[i].id), &(processArray[i].arrivalTime),
        &(processArray[i].runTime), &(processArray[i].priority));
    }
    fclose(filePtr);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.

    char* fileName= argv[1];
    int processCount=getNumberOfLines(fileName);
    Process processArray[processCount];
    readInputFiles(fileName, processArray, processCount);
    
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function. 
    int x = getClk();
    printf("Current Time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
