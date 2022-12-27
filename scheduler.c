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
            printf("last flag of process %d is %d\n", recievedProcess.id, recievedProcess.flagLast);
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
            destroyClk(true);
            break;
        }
        //
    }
}



void hpfAlgorithm()
{
    int lastFlag = 0;
    int RC, clk, receiveState,queuId,msgNum;
    struct msqid_ds buf;
    Process processToRun;
    signal(SIGCHLD, hpfHandler);
    printf("Im in HPF now\n");
    while(true)
    {
        fflush(stdout);
        clk = getClk();
        sleep(1);
        clk = getClk();
        queuId = intMsgQueue(QKEY);
        Process recProcess;
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
        RC = msgctl(queuId,IPC_STAT, &buf);
        msgNum = buf.msg_qnum;

        while (msgNum != 0){
            printf("Current Time is %d\n", getClk());
            recProcess = recieveProcess(queuId, &receiveState);
            if (receiveState != -1)
            {
                printf("at time %d", getClk());
                printf(" process %d",recProcess.id);
                printf(" was received\n");
                lastFlag = recProcess.flagLast;
                if(processPtr == NULL)
                {
                    insert_by_priority(&recProcess);
                }
                else
                {
                    printf("there was a process running, we will stop it using sigstop\n");
                    currentIsFinished = 0;
                    kill(processPtr->forkingID, SIGSTOP);
                    insert_by_priority(&recProcess);
                }
            RC = msgctl(queuId,IPC_STAT, &buf);
            msgNum = buf.msg_qnum;
            }
        }
        if (!runningFlag && !isEmpty())
        {
            runningFlag = 1;
            display_pqueue();
            processToRun = popQueue();
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
            processToRun.state = running;
            kill(processToRun.forkingID, SIGCONT);
            runningProcessID = processToRun.forkingID;
            }
            while (processToRun.remainingTime > 0)
            {
            processToRun.remainingTime--;
            printf("process %d", processToRun.id);
            printf(" is running...\n");
            sleep(1);
            }
        }

        if (isEmpty() && lastFlag && !runningFlag)
        {
            destroyClk(true);
            break;
        }
        
    }
}

void rrAlgorithm(int quantum)
{
    signal(SIGCHLD, handler);
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
                if (recievedProcess.flagLast==1) lastProcess=recievedProcess;
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
            // printf("finished process of remaining time %d\n", finishedProcess.remainingTime);
            if(!lastFlag) lastFlag = finishedProcess.flagLast;
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
                    fprintf(filePtr, "At time %d process %d started arr %d total %d remain %d\n", 
                getClk(),finishedProcess.id,finishedProcess.arrivalTime,finishedProcess.runTime,finishedProcess.remainingTime);
                }
            }
            else
            {
                printf("Process %d Continued \n", finishedProcess.id);
                fprintf(filePtr, "At time %d process %d continued arr %d total %d remain %d\n", 
                getClk(),finishedProcess.id,finishedProcess.arrivalTime,finishedProcess.runTime,finishedProcess.remainingTime);
                kill(finishedProcess.forkId, SIGCONT);
            }
            x = getClk();
            int q=quantum;
            if (finishedProcess.remainingTime<quantum) q=finishedProcess.remainingTime;
            while ((getClk() - x) != q)
            {
                sleep(1);
                finishedProcess.remainingTime -= 1;
                // printf("Current Time is %d\n", getClk());
            }
            sleep(0.01);
            kill(finishedProcess.forkId, SIGSTOP);
        }

    }
   
    printf("Is empty out of while= %d\n",isEmptyQueue());
}

void mlfqAlgorithm(int quantum){
    int lastFlag = 0;
    int rc;
    int x;
    int rState, qState;
    int lastLoop = 0;
    circQueue* MLFQ[11];
    int message_num;
    int currentPriority = 0;
    runningFlag = 0;
    Process recievedProcess,finishedProcess;
    int aProcessFinished = 0;
    for (int i = 0; i<11; i++){
        MLFQ[i] = CreateQueueM();
    }
    int AllEmpty = 1;
    fflush(stdout);
    while(!lastFlag || runningFlag || !(AllEmpty) || aProcessFinished){
        int q = quantum * (currentPriority + 1);
        signal(SIGCHLD, handler);
        fflush(stdout);
        sleep(1);
        x=getClk();
        printf("Current time is %d\n",x);
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
                //todo: insert in the circular queue
                qState=enQueueM(MLFQ[recievedProcess.priority],&recievedProcess);
                if(recievedProcess.priority<currentPriority) currentPriority = recievedProcess.priority;
                printf("enqueued in queue number %d\n",recievedProcess.priority);
                //printf("state of queue %d is %d\n",recievedProcess.priority,isEmptyQueueM(MLFQ[recievedProcess.priority]));
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }

        for(int i = 0; i<11; i++){
            if(!isEmptyQueueM(MLFQ[i])){
                AllEmpty = 0;
                currentPriority = i;
                break;
            }
            AllEmpty = 1;
        }
        printf("is All Empty %d\n",AllEmpty);

        if(aProcessFinished){
            printf("Process %d was enqueued in the priority %d\n",finishedProcess.id,finishedProcess.priority + 1);
            // if(finishedProcess.waitingTime>=20 && !finishedProcess.isBoosted){
            //     printf("process %d got boosted\n",finishedProcess.id);
            //     finishedProcess.isBoosted = 1;
            //     qState = enQueueM(MLFQ[currentPriority - 1],&finishedProcess);
            //     currentPriority--;
            // }
            // else{
                if(currentPriority < 10){
                    qState = enQueueM(MLFQ[currentPriority + 1],&finishedProcess);
                }
                else{
                    qState = enQueueM(MLFQ[10],&finishedProcess);
                }
            // }
            aProcessFinished = 0;
        }

        printf("state of queue %d is %d\n",currentPriority,isEmptyQueueM(MLFQ[currentPriority]));
        if(isEmptyQueueM(MLFQ[currentPriority])){
            if(!AllEmpty){
                if(currentPriority != 10){
                    printf("Moving on to Queue Number %d\n",currentPriority + 1);
                    currentPriority++;
                }
                else{
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

        if(!isEmptyQueueM(MLFQ[currentPriority]) && !runningFlag){
            finishedProcess = deQueueM(MLFQ[currentPriority]);
            finishedProcess.waitingTime = (getClk() - finishedProcess.arrivalTime - (finishedProcess.realTime - finishedProcess.remainingTime) - 1);
            if(finishedProcess.waitingTime>=20 && !finishedProcess.isBoosted){
                printf("process %d got boosted\n",finishedProcess.id);
                finishedProcess.isBoosted = 1;
                qState = enQueueM(MLFQ[currentPriority - 1],&finishedProcess);
                currentPriority--;
            }
            //finishedProcess.waitingTime = 0;
            else{
                printf("Waiting time of Process %d is %d\n",finishedProcess.id,finishedProcess.waitingTime);
                printf("process %d was dequeued\n",finishedProcess.id);
                finishedProcess.state = running;
                if(!lastFlag) lastFlag = finishedProcess.flagLast;
                char remaining[10];

                runningFlag = 1;


                sprintf(remaining, "%d", finishedProcess.remainingTime);

                if(finishedProcess.forkId == 0){

                    int pid = fork();
                    if(pid == 0){
                        run("process", remaining,NULL);
                    } 
                    else{
                        printf("process %d was started\n",finishedProcess.id);
                        finishedProcess.forkId = pid;
                    }
                }
                else{
                    printf("process %d was continued\n",finishedProcess.id);
                    kill(finishedProcess.forkId, SIGCONT);
                }

                x = getClk();
                printf("current quantum is %d\n", q);
                if(finishedProcess.remainingTime<q) {
                    q = finishedProcess.remainingTime;
                    if(!(!lastFlag || runningFlag || !(AllEmpty) || aProcessFinished) && !lastLoop){
                        aProcessFinished = 1;
                        lastLoop = 1;
                    }
                }
                else{
                    aProcessFinished = 1;
                }
                while((getClk() - x) != q) {
                    sleep(1);
                    finishedProcess.remainingTime--;
                    printf("Current time is %d\n",getClk());
                }

                sleep(0.01);
                printf("process %d was stopped\n",finishedProcess.id);
                kill(finishedProcess.forkId,SIGSTOP);
                }
                
        }
        
    }
}

int main(int argc, char *argv[])
{
    initClk();
    create();
    printf("%d is my Parent \n", getppid());
    int chosenAlgorithm = atoi(argv[0]);
    filePtr=fopen("scheduler.log.txt", "w");
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

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    // sleep(4);
    // destroyClk(true);
    printf("%d is my Parent \n", getppid());
    fclose(filePtr);
    destroyClk(true);

    kill(getppid(), SIGINT);
    return 0;
}
