#include "PriorityQueue.h"
#include "CircQueue.h"
#include "MultiCircQueue.h"
//  TODO sort queue to arrival time
int runningFlag = 0;
int pidTracker[100];

void handler(int signum)
{
    runningFlag = 0;
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
        // Process recievedProcess=recieveProcess(queueId, &rState);
        // if (rState!=-1)
        // {
        //     //todo: insert in the circular queue
        //     qState=enQueue(&recievedProcess);
        // }

        // If there are multiple processes in the queue it will enter the while loop
        // Else it will work on the already recievedProcess above

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
                // todo: insert in the circular queue
                // printf("Recieved process: %d\n", recievedProcess.id);
                // printf("Recieved process of remaining time %d\n", recievedProcess.remainingTime);
                qState = enQueue(&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }
        if (aProcessFinished)
        {
            //TO DO WE NEED TO TRANSFER LAST FLAG TO RECENTLY RE-ENQUEUED AND RESET LAST FLAG TO 0
            //FIND ID OF PROCESS WITH LAST FLAG'
            //Pointer on last process that points to the final process
            // lastFlag=0;
            // finishedProcess.flagLast=1;
            // lastProcess.flagLast=0;
            // lastProcess=finishedProcess;
            qState = enQueue(&finishedProcess);
            // printf("Process %d returned to queue \n", finishedProcess.id);
            aProcessFinished = 0;
        }

        // The size of circ queue must be size of the processes

        // while loop in the scheduler to compare quantum with current time remainder
        // then decide whether to

        // Enqueue all processes in the circular queue (the rear will move)
        // until the msg queue is empty
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
            // printf("The remaining Time of process %d is %d \n", finishedProcess.id, finishedProcess.remainingTime);
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
                }
            }
            else
            {

                // printf("ForkID is %d\n", finishedProcess.forkId);
                printf("Process %d Continued \n", finishedProcess.id);
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

        // printf("Is empty= %d\n",isEmptyQueue());
        // printf("Running Flag= %d\n",runningFlag);
        // printf("Last Flag= %d\n",lastFlag);
    }

    /*
    while (not all processes finished)

        check if a new process arrived
        check the remaining of the process
        if remainingTime < quantum -> make remainingTime=0 and exit it
        else

            if not forked (i.e if not in the pidTracker) -> fork, else sigcont to this id
            stop the rest
            run this process
            remainingTime-=quantum
            stop
            check if another one arrived
            put at the end of the queue

    */
    printf("Is empty out of while= %d\n",isEmptyQueue());
}

void mlfqAlgorithm(int quantum){
    int lastflag = 0;
    int rc;
    int x;
    int rState, qState;
    circQueue* MLFQ[11];
    int message_num;
    int currentPriority = 0;
    Process recievedProcess,finishedProcess;
    int aProcessFinished = 0;
    for (int i = 0; i<11; i++){
        MLFQ[i] = CreateQueueM();
    }
    fflush(stdout);
    while(!lastflag){
        signal(SIGCHLD, handler);
        fflush(stdout);
        sleep(1);
        x=getClk();
        int queueId= intMsgQueue(QKEY);
        struct msqid_ds buf;
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            sleep(0.01);
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                //todo: insert in the circular queue
                qState=enQueueM(MLFQ[recievedProcess.priority],&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }

        if(aProcessFinished){
            qState = enQueueM(MLFQ[finishedProcess.priority + 1],&finishedProcess);
        }

        if(isEmptyQueueM(MLFQ[currentPriority])){
            if(currentPriority != 10){
                currentPriority++;
            }
            else{
                currentPriority = 0;
            }
        }
        if(!isEmptyQueueM(MLFQ[currentPriority]) && !runningFlag){
            finishedProcess = deQueueM(MLFQ[currentPriority]);
            finishedProcess.state = running;
            if(!lastflag) lastflag = finishedProcess.flagLast;
            char remaining[10];

            runningFlag = 1;

            sprintf(remaining, "%d", finishedProcess.remainingTime);

            if(finishedProcess.forkId == 0){
                int pid = fork();
                if(pid == 0) run("process", remaining,NULL);
                else{
                    finishedProcess.forkId = pid;
                }
            }
            else{
                kill(finishedProcess.forkId, SIGCONT);
            }

            x = getClk();
            int q = quantum * (currentPriority + 1);
            if(finishedProcess.remainingTime<q) q =finishedProcess.remainingTime;
            while((getClk() - x) != q) {
                sleep(1);
                finishedProcess.remainingTime--;
            }

            sleep(0.01);
            kill(finishedProcess.forkId,SIGSTOP);
        }
        
    }
}

int main(int argc, char *argv[])
{
    initClk();
    create();
    printf("%d is my Parent \n", getppid());
    int chosenAlgorithm = atoi(argv[0]);

    if (chosenAlgorithm == SJF)
    {
        setKey(runTime);
        sjfAlgorithm();
    }
    else if (chosenAlgorithm == RR)
    {
        rrAlgorithm(atoi(argv[1]));
        // printf("I")
    }
    else if(chosenAlgorithm == MLFL){
        mlfqAlgorithm(atoi(argv[1]));
    }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    // sleep(4);
    // destroyClk(true);
    printf("%d is my Parent \n", getppid());
    destroyClk(true);

    kill(getppid(), SIGINT);
    return 0;
}
