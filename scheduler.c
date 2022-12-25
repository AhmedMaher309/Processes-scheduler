#include "PriorityQueue.h"
// #include "MultiCircQueue.h"
//  TODO sort queue to arrival time
int runningFlag = 0;
int pidTracker[100];
int ProcessRemainingTIme =0;
int runningProcessID;
int currentIsFinished = -1;
Process * processPtr = NULL;


void handler(int signum)
{
    runningFlag = 0;
    if(currentIsFinished == 1)
    {
        printf("A process has been terminated normally\n");
        if (processPtr != NULL)
        {
         //  insert_by_priority(processPtr);
           printf("process %d",processPtr->id);
           printf(" inserted again\n");
           processPtr = NULL;
        }
    }
    else{
        printf("A process has been preempted\n");
        display_pqueue();
        insert_by_priority(processPtr);
        currentIsFinished = 1;
    }

    
}

void sjfAlgorithm()
{
    int lastFlag = 0;
    int rc;
    int x;
    int rState;
    fflush(stdout);

    while (1)
    {
        signal(SIGCHLD, handler);
        fflush(stdout);
        x = getClk();
        printf("Current Time is %d\n", x);
        sleep(1);
        x = getClk();
        int queueId = intMsgQueue(QKEY);
        struct msqid_ds buf;
        sleep(0.2);
        Process recievedProcess = recieveProcess(queueId, &rState);
        if (rState != -1)
        {
            printf("id of recieved process: %d\n", recievedProcess.id);
            lastFlag = recievedProcess.flagLast;
            printf("last flag of process %d is %d\n", recievedProcess.id, recievedProcess.flagLast);
            insert_by_priority(&recievedProcess);
        }
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                lastFlag = recievedProcess.flagLast;
                printf("last flag of process %d is %d\n", recievedProcess.id, recievedProcess.flagLast);
                insert_by_priority(&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }
        do
        {
            if (!runningFlag && !isEmpty())
            {
                if (lastFlag && !isEmpty())
                {
                    printf("Current Time is %d\n", x);
                }
                display_pqueue();
                recievedProcess = popQueue();
                display_pqueue();
                printf("rState = %d \n", rState);
                recievedProcess.state = running;
                int currentProcess = recievedProcess.id;
                printf("process %d entered\n", recievedProcess.id);
                // lastFlag = recievedProcess.flagLast;
                char remain[10];
                sprintf(remain, "%d", recievedProcess.runTime);
                runningFlag = 1;
                int processPid = fork();
                fflush(stdout);
                if (processPid == 0)
                {
                    printf("This is the child \n");
                    printf("This is great \n");
                    fflush(stdout);
                    run("process", remain, NULL);
                }
           }

        } while (!lastFlag && !isEmpty());

        if (lastFlag && isEmpty())
        {
            int lastArriv = getClk();
            while (getClk() != lastArriv + recievedProcess.runTime + 1)
            {
                printf("Current Time is %d\n", getClk());
                sleep(1);
            }
            destroyClk(true);
            break;
        }
    }
}



void hpfAlgorithm()
{
    int lastFlag = 0;
    int RC, clk, receiveState,queuId,msgNum;
    struct msqid_ds buf;
    Process processToRun;
    signal(SIGCHLD, handler);
    printf("Im in HPF now\n");
    while(true)
    {
        fflush(stdout);
        clk = getClk();
        sleep(1);
        printf("Current Time is %d\n", clk);
        //sleep(1);
        clk = getClk();
        queuId = intMsgQueue(QKEY);
        Process recProcess = recieveProcess(queuId, &receiveState);
        if (receiveState != -1)
        {
            //printf("remain time %d\n", recProcess.remainingTime);
            lastFlag = recProcess.flagLast;
            printf("id of the received process is: %d", recProcess.id);
            printf(" and its flag is: %d\n",recProcess.flagLast);
            if(processPtr == NULL)
            {
                insert_by_priority(&recProcess);
            }
            else{
                printf("there was a process running, we will stop it using sigstop\n");
                currentIsFinished = 0;
                kill(processPtr->forkingID, SIGSTOP);
                printf("we stopped process %d\n",processPtr->id);
                insert_by_priority(&recProcess);
            }
        }
        RC = msgctl(queuId,IPC_STAT, &buf);
        msgNum = buf.msg_qnum;

        while (msgNum != 0){
            printf("Current Time is %d\n", clk);
            recProcess = recieveProcess(queuId, &receiveState);
            if (receiveState != -1)
            {
                lastFlag = recProcess.flagLast;
                printf("id of the received process is: %d", recProcess.id);
                printf(" and its flag is: %d\n",recProcess.flagLast);
                if(processPtr == NULL)
                {
                    insert_by_priority(&recProcess);
                }
                else
                {
                    printf("there was a process running, we will stop it using sigstop\n");
                    currentIsFinished = 0;
                    kill(processPtr->forkingID, SIGSTOP);
                    printf("we stopped process %d\n",processPtr->id);
                    insert_by_priority(&recProcess);
                }
            RC = msgctl(queuId,IPC_STAT, &buf);
            msgNum = buf.msg_qnum;
            }
        }
        if(!runningFlag && !isEmpty())
        {
            display_pqueue();
            printf("Current Time is %d\n", clk);
            runningFlag = 1;
            processToRun = popQueue();
            processPtr = &processToRun;
            processToRun.state = started;
            processToRun.startingTime = getClk();
            printf("process forking id = %d\n", processToRun.forkingID);

            // the process hasn't been forked before
            if (processToRun.forkingID == -1)
            {
                char remain[10];
                sprintf(remain, "%d", processToRun.runTime);
                runningFlag = 1;
                runningProcessID = fork();
                if(runningProcessID != 0)
                {
                    processToRun.forkingID = runningProcessID;
                }
                if (runningProcessID == 0)
                {
                    printf("im in the forking\n");
                    printf("the running process is %d\n", processToRun.id);
                    run("process", remain, NULL);
                }
            }

            // the process has been forked before
            else{
                printf("lets continue the process with id %d\n", processToRun.id);
                processToRun.state = running;
                kill(processToRun.forkingID, SIGCONT);
                runningProcessID = processToRun.forkingID;
            }
        }

        if (isEmpty() && lastFlag && !runningFlag)
        {
            destroyClk(true);
            break;
        }
        
    }
}



int main(int argc, char *argv[])
{
    initClk();
    create();
    printf("%d is my Parent \n", getppid());
    int chosenAlgorithm = atoi(argv[0]);

    if (chosenAlgorithm == 3)
    {
        setKey(priority);
        hpfAlgorithm();
    }
    // else if (chosenAlgorithm==RR)
    // {
    //     rrAlgorithm(atoi(argv[1]));
    // }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    // sleep(4);
    // destroyClk(true);
    printf("%d is my Parent \n", getppid());
    kill(getppid(), SIGINT);
    // destroyClk();
    return 0;
}




// gcc process_generator.c -o process_gen.out 
// ./process_gen.out processes.txt  -sch 3 -q 5


// if the sigchild is from stopping the proc4ss ---> ignore it
// if it is from the process termination ----> change the flag to return to its normal and 
// not put that process in the scheduler  then continue