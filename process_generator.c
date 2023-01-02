#include "headers.h"

int clockPid, schedulePid;
void clearResources(int);
int getNumberOfLines(char *fileName)
{
    int count = 0;
    FILE *filePtr = fopen(fileName, "r");
    char temp;
    temp = getc(filePtr);
    while (temp != EOF)
    {
        if (temp == '\n')
            count++;
        temp = getc(filePtr);
    }
    fclose(filePtr);
    return count;
}

void readInputFiles(char *fileName, Process *processArray, int processCount)
{
    FILE *filePtr = fopen(fileName, "r");
    // Skip first line
    fscanf(filePtr, "%*[^\n]\n");
    for (int i = 0; i < processCount; i++)
    {
        processArray[i] = *createProcess(0, 0, 0, 0);
        fscanf(filePtr, "%d %d %d %d %d", &(processArray[i].id), &(processArray[i].arrivalTime),
               &(processArray[i].runTime), &(processArray[i].priority), &(processArray[i].memSize));
        processArray[i].remainingTime = processArray[i].runTime;
        //processArray[i].realTime = processArray[i].runTime;
    }
    fclose(filePtr);
}

int main(int argc, char *argv[])
{
    // signal(SIGINT, clearResources);
    //  TODO Initialization
    //  1. Read the input files.

    char *fileName = argv[1];
    int processCount = getNumberOfLines(fileName);
    Process processArray[processCount];
    readInputFiles(fileName, processArray, processCount);
    printf("process count = %d\n", processCount);
    //  2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    //  3. Initiate and create the scheduler and clock processes.
    //  4. Use this function after creating the clock process to initialize clock.

    clockPid = fork();
    if (clockPid == 0) // child(clock process)
    {
        // printf("go to run \n");
        run("clk", NULL, NULL);
    }

    enum Algorithm chosenAlgorithm = atoi(argv[3]);
    int quantum;
    if (chosenAlgorithm == RR || chosenAlgorithm == MLFL)
        quantum = atoi(argv[5]);

    schedulePid = fork();
    if (schedulePid == 0) // child(scheduler process)
    {

        if (chosenAlgorithm == RR || chosenAlgorithm == MLFL)
        {
            // printf("go to scheduler \n");
            run("scheduler", argv[3], argv[5]);
        }
        else
        {
            run("scheduler", argv[3], NULL);
        }
    }

    //  To get time use this function.
    int x;
    int currentProcess = 0;
    int msgqId = intMsgQueue(QKEY); // This will create message queue (will be used between process generator and scheduler)
    initClk();
    // 5. Create a data structure for processes and provide it with its parameters.
    // TODO Generation Main Loop
    while (currentProcess < processCount)
    {
        // x = getClk();
        sleep(1);
        x = getClk();
        // printf("Current Time is %d\n", x);
        fflush(stdout);                                       // 6. Send the information to the scheduler at the appropriate time.
        while (processArray[currentProcess].arrivalTime == x) // made it while not if, because I want the parent process not to procced until it, finishes the current x
        {
            printf("process number %d\n", processArray[currentProcess].id);
            // printf("process count %d\n", processCount);

            if (currentProcess == processCount - 1)
            {
                processArray[currentProcess].flagLast = 1;
                printf("%d is last flag", processArray[currentProcess].flagLast);
            }
            sendProcess(processArray[currentProcess], msgqId);
            currentProcess++;
        }
    }
    // 7. Clear clock resources
    int finished, stat_loc;
    finished = waitpid(schedulePid, &stat_loc, 0);
    if (!(stat_loc & 0x00FF))
    {
        fflush(stdout);
        printf("%d ", stat_loc >> 8);
    }
    

    
    destroyClk(true); 
    clearResources(SIGINT);
}

void clearResources(int signum)
{
    //     // TODO Clears all resources in case of interruption
    //     //Delete the queue
    //     //Destroy the clk
    //     //Call clear resources handler instead of destroyClk(true)

        kill(clockPid, SIGINT);
        int msgqId = intMsgQueue(QKEY);
        struct msqid_ds buf;
        msgctl (msgqId, IPC_RMID, &buf );
        printf("Process_generator of PID = %d exited\n", getpid());
        exit(0);
        signal( SIGINT, clearResources);
}
