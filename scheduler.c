#include "PriorityQueue.h"
// Header is included in the priority queue
//  TODO sort queu to arrival time
int runningFlag = 0;
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
    // printf("check this \n");
    while (!lastFlag)
    {
        signal(SIGCHLD, handler);
        fflush(stdout);
        x = getClk();
        sleep(1);
        x = getClk();
        int queueId = intMsgQueue(QKEY);

        // // اللي يجيلك هتحطه في البرايورتي كيو وطول ما هي مش فاضية هتديقيو منها
        // // ولو فيها أكتر من اليمنت هتسحب اللي عالوش
        // // If not empty pop first element

        struct msqid_ds buf;
        // int message_num

        sleep(0.02);
        Process recievedProcess = recieveProcess(queueId, &rState);
        if (rState != -1)
        {
            printf("id of recieved process: %d\n", recievedProcess.id);
            insert_by_priority(&recievedProcess);
            // display_pqueue();
        }
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;

        // printf("msg queue count: %d\n", message_num);

        while (message_num != 0)
        {
            sleep(0.01);
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                insert_by_priority(&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }

        if (!runningFlag && !isEmpty())
        {
            display_pqueue();
            // pop when the running flag = 0 (running flag will be determined from handler of SIGCHILD)
            recievedProcess = popQueue();
            printf("rState = %d \n", rState);
            display_pqueue();
            // printf("Run time of Popped process: %d\n", recievedProcess.runTime);
            recievedProcess.state = running;
            // fflush(stdout);
            // printf("startingTime= %d\n", getClk());
            // if (recievedProcess.state == running)
            // {
            // fflush(stdout);
            printf("process %d entered\n", recievedProcess.id);
            lastFlag = recievedProcess.flagLast;
            // printf("Last Flag: %d\n", lastFlag);
            char remain[10];
            // fflush(stdout);
            sprintf(remain, "%d", recievedProcess.runTime);
            // printf("remaining time = %d\n", recievedProcess.runTime);
            runningFlag = 1;
            int processPid = fork();
            // printf("fork pid: %d\n", processPid);
            fflush(stdout);
            if (processPid == 0)
            {
                printf("This is the child \n");
                printf("This is great \n");

                run("process", remain, NULL);
            }

            // if (processPid == 0)
            // {
            //     // fflush(stdout);
            //     printf("I am the fucking process");
            //     run("process", NULL, NULL);
            // }
            // }
        }
    }
}

int main(int argc, char *argv[])
{
    initClk();
    create();
    if (atoi(argv[0]) == 3)
    {
        setKey(runTime);
        sjfAlgorithm();
    }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    sleep(4);
    // destroyClk(true);
}
