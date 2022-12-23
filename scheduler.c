#include "PriorityQueue.h"
// #include "MultiCircQueue.h"
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
    printf("Im in HPF now\n");
    while(true)
    {
        signal(SIGCHLD, handler);
        fflush(stdout);
        clk = getClk();
        printf("Current Time is %d\n", clk);
        sleep(1);
        clk = getClk();
        queuId = intMsgQueue(QKEY);
        Process recProcess = recieveProcess(queuId, &receiveState);
        if (receiveState != -1)
        {
            printf("id of the received process is: %d", recProcess.id);
            printf(" and its flag is: %d\n",recProcess.flagLast);
            insert_by_priority(&recProcess);
        }
        RC = msgctl(queuId,IPC_STAT, &buf);
        msgNum = buf.msg_qnum;

        while (msgNum != 0){
            printf("Current Time is %d\n", clk);
            recProcess = recieveProcess(queuId, &receiveState);
            if (receiveState != -1)
            {
            printf("id of the received process is: %d", recProcess.id);
            printf(" and its flag is: %d\n",recProcess.flagLast);
            insert_by_priority(&recProcess);
            RC = msgctl(queuId,IPC_STAT, &buf);
            msgNum = buf.msg_qnum;
            }
        }
        do{
            if (!runningFlag && !isEmpty())
            {
                if (lastFlag && !isEmpty())
                {
                    printf("current time is: %d", getClk());
                }
                printf("queue values are: ");
                display_pqueue();
                recProcess = popQueue();
                //printf("state of msg queue is: %d\n", receiveState);
                char remain[10];
                sprintf(remain, "%d", recProcess.runTime);
                runningFlag = 1;
                int processId = fork();
                if(processId == 0)
                {
                    printf("process forked successfuly\n");
                    run("process", remain, NULL);
                }
            }

        }while(!lastFlag && ! isEmpty());
        if (lastFlag && isEmpty())
        {
            int lastArriv = getClk();
            while (getClk() != lastArriv + recProcess.runTime + 1)
            {
                printf("Current Time is %d\n", getClk());
                sleep(1);
            }
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
        setKey(runTime);
        sjfAlgorithm();
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
