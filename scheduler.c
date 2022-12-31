#include "PriorityQueue.h"
#include "CircQueue.h"
#include "MultiCircQueue.h"
//  TODO sort queue to arrival time
int runningFlag = 0;
int pidTracker[100];
int ProcessRemainingTIme =0;
int runningProcessID;
int currentIsFinished = -1;
Process * processPtr = NULL;
Process finishedProcesses[100];
int index1 = 0;
float lagTime = 0;

FILE *filePtr;  

void handler(int signum)
{
    runningFlag = 0;
}

void hpfHandler(int signum)
{
    runningFlag = 0;
    if(currentIsFinished == 1)
    {
        // printf("A process has been terminated normally");
        // printf(" at time %d\n", getClk());
        finishedProcesses[index1] = *processPtr;
        index1++;
        if (processPtr != NULL)
        {
           processPtr = NULL;
        }
    }
    else{
        printf("process %d",processPtr->id);
        printf(" has been preempted");
        printf(" at time %d\n",getClk());
        display_pqueue();
        insert_by_priority(processPtr);
        processPtr = NULL;
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
            //printf("last flag of process %d is %d\n", recievedProcess.id, recievedProcess.flagLast);
            insert_by_priority(&recievedProcess);
        }
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            // sleep(0.1);
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                lastFlag = recievedProcess.flagLast;
                //printf("last flag of process %d is %d\n", recievedProcess.id, recievedProcess.flagLast);
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

                //display_pqueue();
                recievedProcess = popQueue();
                //display_pqueue();
                //printf("rState = %d \n", rState);
                recievedProcess.state = running;
                // if(recievedProcess.id != NULL){
                //     fprintf(filePtr, "At time %d process %d finished arr %d total %d remain %d wait %d\n", 
                // getClk(),recievedProcess.id,recievedProcess.arrivalTime,recievedProcess.runTime,recievedProcess.remainingTime, recievedProcess.waitingTime);
                // }
                int currentProcess = recievedProcess.id;
                printf("process %d entered\n", recievedProcess.id);
                // lastFlag = recievedProcess.flagLast;
                char remain[10];
                //printf("******* RunTime=%d\n",recievedProcess.runTime);
                sprintf(remain, "%d", recievedProcess.runTime);
                runningFlag = 1;
                //fprintf(filePtr,"Sherif\n");
                recievedProcess.waitingTime = (getClk() - recievedProcess.arrivalTime - (recievedProcess.runTime - recievedProcess.remainingTime) - 1);
                finishedProcesses[index1] = recievedProcess;
                index1++;
                fprintf(filePtr, "At time %d process %d started arr %d total %d remain %d wait %d\n", 
                getClk(),recievedProcess.id,recievedProcess.arrivalTime,recievedProcess.runTime,recievedProcess.remainingTime, recievedProcess.waitingTime);
                int processPid = fork();
                fflush(stdout);
                if (processPid == 0)
                {
                    //printf("This is the child \n");
                    //printf("This is great \n");
                    fflush(stdout);
                    run("process", remain, NULL);
                }
            }
            // if (lastFlag && isEmpty())
            // {
            //     destroyClk(true);
            //     break;
            // }

        } while (!lastFlag && !isEmpty());
        if (lastFlag && isEmpty())
        {
            int lastArriv = getClk();
            while (getClk() != lastArriv + recievedProcess.runTime + 1)
            {
                printf("Current Time is %d\n", getClk());
                sleep(1);
            }
            //destroyClk(true);
            break;
        }
        lagTime++;
        //
    }
}





void hpfAlgorithm()
{
    int lastFlag = 0;
    int RC, clk, receiveState, queuId, msgNum;
    struct msqid_ds buf;
    Process processToRun;
    signal(SIGCHLD, hpfHandler);
    printf("Im in HPF now\n");
    while (true)
    {
        fflush(stdout);
        clk = getClk();
        sleep(1);
        clk = getClk();
        queuId = intMsgQueue(QKEY);
        Process recProcess;
        RC = msgctl(queuId, IPC_STAT, &buf);
        msgNum = buf.msg_qnum;

        while (msgNum != 0)
        {
            printf("Current Time is %d\n", getClk());
            recProcess = recieveProcess(queuId, &receiveState);
            if (receiveState != -1)
            {
                printf("at time %d", getClk());
                printf(" process %d", recProcess.id);
                printf(" was received\n");
                lastFlag = recProcess.flagLast;
                if (processPtr == NULL)
                {
                    insert_by_priority(&recProcess);
                }
                else
                {
                    // printf("there was a process running, we will stop it using sigstop\n");
                    currentIsFinished = 0;
                    fprintf(filePtr, "At time %d process %d stopped at %d total %d remain %d wait %d\n",
                            getClk(), processPtr->id, processPtr->arrivalTime, processPtr->runTime, processPtr->remainingTime, processPtr->waitingTime);
                    kill(processPtr->forkingID, SIGSTOP);
                    insert_by_priority(&recProcess);
                }
                RC = msgctl(queuId, IPC_STAT, &buf);
                msgNum = buf.msg_qnum;
            }
        }
        if (!runningFlag && !isEmpty())
        {
            runningFlag = 1;
            display_pqueue();

            processToRun = popQueue();
            processToRun.waitingTime = (getClk() - processToRun.arrivalTime - (processToRun.runTime - processToRun.remainingTime) - 1);
            processPtr = &processToRun;
            processToRun.state = started;
            processToRun.startingTime = getClk();
            if (processToRun.forkingID == -1)
            {
                char remain[10];
                sprintf(remain, "%d", processToRun.runTime);
                // runningFlag = 1;
                runningProcessID = fork();
                if (runningProcessID != 0)
                {
                    processToRun.forkingID = runningProcessID;
                    printf("process is %d", processToRun.id);
                    printf(" resumed running at time %d\n", getClk());
                    fprintf(filePtr, "At time %d process %d started at %d total %d remain %d wait %d\n",
                            getClk(), processToRun.id, processToRun.arrivalTime, processToRun.runTime, processToRun.remainingTime, processToRun.waitingTime);
                }
                if (runningProcessID == 0)
                {

                    printf("the running process is %d", processToRun.id);
                    printf(" at time %d\n", getClk());
                    run("process", remain, NULL);
                }
                processToRun.remainingTime--;
            }
        

            // the process has been forked before
            else
            {
                printf("lets continue the process with id %d\n", processToRun.id);
                printf("this process forking id is %d\n", processToRun.forkingID);
                fprintf(filePtr, "At time %d process %d resumed at %d total %d remain %d wait %d\n",
                        getClk(), processToRun.id, processToRun.arrivalTime, processToRun.runTime, processToRun.remainingTime, processToRun.waitingTime);
                processToRun.state = running;
                kill(processToRun.forkingID, SIGCONT);
                printf("process %d", processToRun.id);
                printf(" is running...\n");
                while (processToRun.remainingTime > 0)
                {
                    processToRun.remainingTime--;
                    sleep(1);
                }
                runningProcessID = processToRun.forkingID;
            }
        }
        printf("time now is %d\n", getClk());
        if (isEmpty() && lastFlag && !runningFlag)
        {
            //destroyClk(true);
            break;
        }
    }
    lagTime++;
}

void rrAlgorithm(int quantum)
{
    signal(SIGCHLD, handler);
    fflush(stdout);
    int lastFlag = 0;
    int rc, x, rState, qState;
    int aProcessFinished = 0;
    Process finishedProcess, recievedProcess, lastProcess;
    while (!lastFlag || runningFlag || !(isEmptyQueue()) || aProcessFinished)
    {
        sleep(1);
        x = getClk();
        printf("Current Time is %d\n", x);
        int queueId = intMsgQueue(QKEY);

        // while loop to recieve
        struct msqid_ds buf;
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            sleep(0.01);
            recievedProcess = recieveProcess(queueId, &rState);
            if (rState != -1)
            {
                if (recievedProcess.flagLast == 1)
                    lastProcess = recievedProcess;
                qState = enQueue(&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }
        if (aProcessFinished)
        {
            qState = enQueue(&finishedProcess);
            aProcessFinished = 0;
        }

        if (!(isEmptyQueue()) && !runningFlag)
        {
            finishedProcess = deQueue();
            finishedProcess.state = running;
            printf("Process %d is dequeed \n", finishedProcess.id);
            finishedProcess.waitingTime = (getClk() - finishedProcess.arrivalTime - (finishedProcess.runTime - finishedProcess.remainingTime) - 1);
            // printf("finished process of remaining time %d\n", finishedProcess.remainingTime);
            if (!lastFlag)
                lastFlag = finishedProcess.flagLast;
            char remaining[10];

            runningFlag = 1;
            /*
              if rt =< quantum -> send remainingTime and don't put it back in the queue
              if rt > quantum -> mark it as returning back to the queue
          */
            if (finishedProcess.remainingTime > quantum)
            {
                // printf("Remainng time is larger than quatum \n");
                aProcessFinished = 1;
            }
            // This will tell us that the process is returning again to the queue

            sprintf(remaining, "%d", finishedProcess.remainingTime);
            if (finishedProcess.forkId == 0)
            {
                printf("Process %d Started \n", finishedProcess.id);
                int pid = fork();

                if (pid == 0)
                    run("process", remaining, NULL);
                else
                {

                    finishedProcess.forkId = pid;
                    printf("forkID is %d\n", finishedProcess.forkId);
                    fprintf(filePtr, "At time %d process %d started at %d total %d remain %d wait %d\n",
                            getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                }
            }
            else
            {
                printf("Process %d Continued \n", finishedProcess.id);
                fprintf(filePtr, "At time %d process %d resumed at %d total %d remain %d wait %d\n",
                        getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                kill(finishedProcess.forkId, SIGCONT);
            }
            x = getClk();
            int q = quantum;
            if (finishedProcess.remainingTime < quantum)
                q = finishedProcess.remainingTime;
            while ((getClk() - x) != q)
            {
                sleep(1);
                finishedProcess.remainingTime -= 1;
                 printf("Current Time is %d\n", getClk());
            }
            if (finishedProcess.remainingTime != 0)
            {
                sleep(0.01);
                kill(finishedProcess.forkId, SIGSTOP);
                fprintf(filePtr, "At time %d process %d stopped at %d total %d remain %d wait %d\n",
                        getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
            }
            else
            {
                sleep(0.01);
                if (finishedProcess.remainingTime <= 0)
                {
                    finishedProcess.remainingTime = 0;
                }
                fprintf(filePtr, "At time %d process %d finished at %d total %d remain %d wait %d\n",
                        getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                // finishedProcesses[index1] = finishedProcess;
                // index1++;
            }
        }
    }
    lagTime++;
    printf("Is empty out of while= %d\n", isEmptyQueue());
}


void mlfqAlgorithm(int quantum)
{
    int lastFlag = 0;
    int rc;
    int x;
    int rState, qState;
    int lastLoop = 0;
    circQueue *MLFQ[11];
    int message_num;
    int currentPriority = 0;
    runningFlag = 0;
    Process recievedProcess, finishedProcess;
    int aProcessFinished = 0;
    for (int i = 0; i < 11; i++)
    {
        MLFQ[i] = CreateQueueM();
    }
    int AllEmpty = 1;
    fflush(stdout);
    while (!lastFlag || runningFlag || !(AllEmpty) || aProcessFinished)
    {
        int q = quantum * (currentPriority + 1);
        signal(SIGCHLD, handler);
        fflush(stdout);
        sleep(1);
        x = getClk();
        printf("Current time is %d\n", x);
        fflush(stdout);
        int queueId = intMsgQueue(QKEY);
        struct msqid_ds buf;
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            sleep(0.01);
            fflush(stdout);
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                // todo: insert in the circular queue
                qState = enQueueM(MLFQ[recievedProcess.priority], &recievedProcess);
                if (recievedProcess.priority < currentPriority)
                    currentPriority = recievedProcess.priority;
                printf("enqueued in queue number %d\n", recievedProcess.priority);
                // printf("state of queue %d is %d\n",recievedProcess.priority,isEmptyQueueM(MLFQ[recievedProcess.priority]));
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }

        for (int i = 0; i < 11; i++)
        {
            if (!isEmptyQueueM(MLFQ[i]))
            {
                AllEmpty = 0;
                currentPriority = i;
                break;
            }
            AllEmpty = 1;
        }
        printf("is All Empty %d\n", AllEmpty);

        if (aProcessFinished)
        {
            printf("Process %d was enqueued in the priority %d\n", finishedProcess.id, finishedProcess.priority + 1);
            // if(finishedProcess.waitingTime>=20 && !finishedProcess.isBoosted){
            //     printf("process %d got boosted\n",finishedProcess.id);
            //     finishedProcess.isBoosted = 1;
            //     qState = enQueueM(MLFQ[currentPriority - 1],&finishedProcess);
            //     currentPriority--;
            // }
            // else{
            if (currentPriority < 10)
            {
                qState = enQueueM(MLFQ[currentPriority + 1], &finishedProcess);
            }
            else
            {
                qState = enQueueM(MLFQ[10], &finishedProcess);
            }
            // }
            aProcessFinished = 0;
        }

        printf("state of queue %d is %d\n", currentPriority, isEmptyQueueM(MLFQ[currentPriority]));
        if (isEmptyQueueM(MLFQ[currentPriority]))
        {
            if (!AllEmpty)
            {
                if (currentPriority != 10)
                {
                    printf("Moving on to Queue Number %d\n", currentPriority + 1);
                    currentPriority++;
                }
                else
                {
                    currentPriority = 0;
                }
                q = quantum * (currentPriority + 1);
            }
        }

        // for(int i = 0; i<currentPriority; i++){
        //     if(!isEmptyQueueM(MLFQ[i])){
        //         currentPriority = i;
        //         break;
        //     }
        // }

        if (!isEmptyQueueM(MLFQ[currentPriority]) && !runningFlag)
        {
            finishedProcess = deQueueM(MLFQ[currentPriority]);
            finishedProcess.waitingTime = (getClk() - finishedProcess.arrivalTime - (finishedProcess.runTime - finishedProcess.remainingTime) - 1);
            if (finishedProcess.waitingTime >= 20 && !finishedProcess.isBoosted)
            {
                printf("process %d got boosted\n", finishedProcess.id);
                finishedProcess.isBoosted = 1;
                qState = enQueueM(MLFQ[currentPriority - 1], &finishedProcess);
                currentPriority--;
            }
            // finishedProcess.waitingTime = 0;
            else
            {
                printf("Waiting time of Process %d is %d\n", finishedProcess.id, finishedProcess.waitingTime);
                printf("process %d was dequeued\n", finishedProcess.id);
                finishedProcess.state = running;
                if (!lastFlag)
                    lastFlag = finishedProcess.flagLast;
                char remaining[10];

                runningFlag = 1;

                sprintf(remaining, "%d", finishedProcess.remainingTime);

                if (finishedProcess.forkId == 0)
                {

                    fprintf(filePtr, "At time %d process %d started at %d total %d remain %d wait %d\n",
                            getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                    int pid = fork();
                    if (pid == 0)
                    {

                        run("process", remaining, NULL);
                    }
                    else
                    {
                        printf("process %d was started\n", finishedProcess.id);
                        finishedProcess.forkId = pid;
                    }
                }
                else
                {
                    fprintf(filePtr, "At time %d process %d resumed at %d total %d remain %d wait %d\n",
                            getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);

                    printf("process %d was resumed\n", finishedProcess.id);
                    kill(finishedProcess.forkId, SIGCONT);
                }

                x = getClk();
                printf("current quantum is %d\n", q);
                if (finishedProcess.remainingTime < q)
                {
                    q = finishedProcess.remainingTime;
                    if (!(!lastFlag || runningFlag || !(AllEmpty) || aProcessFinished) && !lastLoop)
                    {
                        aProcessFinished = 1;
                        lastLoop = 1;
                    }
                }
                else
                {
                    aProcessFinished = 1;
                }
                while ((getClk() - x) != q)
                {
                    sleep(1.001);
                    finishedProcess.remainingTime--;
                    printf("Current time is %d\n", getClk());
                }
                if (finishedProcess.remainingTime > 0)
                {
                    sleep(0.01);
                    printf("process %d was stopped\n", finishedProcess.id);
                    kill(finishedProcess.forkId, SIGSTOP);
                    fprintf(filePtr, "At time %d process %d stopped at %d total %d remain %d wait %d\n",
                            getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                }
                else
                {
                    if (finishedProcess.remainingTime <= 0)
                    {
                        finishedProcess.remainingTime = 0;
                    }
                    fprintf(filePtr, "At time %d process %d finished at %d total %d remain %d wait %d\n",
                            getClk(), finishedProcess.id, finishedProcess.arrivalTime, finishedProcess.runTime, finishedProcess.remainingTime, finishedProcess.waitingTime);
                    finishedProcesses[index1] = finishedProcess;
                    index1++;
                }
            }
        }
        lagTime++;
    }
}
int main(int argc, char *argv[])
{
    initClk();
    create();
    printf("%d is my Parent \n", getppid());
    int chosenAlgorithm = atoi(argv[0]);
    printf("***********************\n");
    filePtr=fopen("scheduler1.log", "w");
    if(filePtr == NULL)
    {
        printf("Error in open the file\n");
    }
    printf("loogging...\n");
    fprintf(filePtr, "#At time x process y state arr w total z remain y wait k\n");
    if (chosenAlgorithm == SJF)
    {
        setKey(runTime);
        sjfAlgorithm();
    }
    else if (chosenAlgorithm==HPF)
    {
        setKey(priority);
        hpfAlgorithm();
    }
    else if (chosenAlgorithm == RR)
    {
        rrAlgorithm(atoi(argv[1]));
        printf("**************************************");
    }
    else if(chosenAlgorithm == MLFL){
        mlfqAlgorithm(atoi(argv[1]));
    }
    lagTime =
    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    // sleep(4);
    // destroyClk(true);
    printf("%d is my Parent \n", getppid());
    fclose(filePtr);
    //printf("Clock is finished \n");
    
    filePtr = fopen("scheduler1.perf", "w");
    float util = ((getClk() - lagTime*0.1)/(getClk())) *100;
    float totalWaitingtime =0;
    float totalWTA = 0;
    for (int i = 0; i < index1; i++)
    {
        totalWaitingtime += finishedProcesses[i].waitingTime;
        totalWTA += finishedProcesses[i].waitingTime / finishedProcesses[i].runTime;
    }
    float avgW = totalWaitingtime/index1;
    fprintf(filePtr,"CPU_utilization = %f \n WTA = %f \n Average waiting time = %f",util,totalWTA,avgW);
    fclose(filePtr);
    printf("%f\n",lagTime);
    kill(getppid(), SIGINT);
    destroyClk(true);
    return 0;
}






//  = recieveProcess(queuId, &receiveState);
        // if (receiveState != -1)
        // {
        //     printf("at time %d", getClk());
        //     printf(" process %d",recProcess.id);
        //     printf(" was received\n");
        //     lastFlag = recProcess.flagLast;
        //     if(processPtr == NULL)
        //     {
        //         insert_by_priority(&recProcess);
        //     }
        //     else{
        //         currentIsFinished = 0;
        //         kill(processPtr->forkingID, SIGSTOP);
        //         printf("we stopped process %d\n",processPtr->id);
        //         insert_by_priority(&recProcess);
        //     }
        // }