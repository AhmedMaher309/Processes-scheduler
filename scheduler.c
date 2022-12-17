#include "PriorityQueue.h"
//Header is included in the priority queue
// TODO sort queu to arrival time

void sjfAlgorithm()
{
    int lastFlag = 0;
    int rc;
    int x;
    fflush(stdout);
    // printf("check this \n");
    while (!lastFlag)
    {
        fflush(stdout);
        x = getClk();
        sleep(1);
        x = getClk();
        int queueId = intMsgQueue(QKEY);

        // اللي يجيلك هتحطه في البرايورتي كيو وطول ما هي مش فاضية هتديقيو منها
        // ولو فيها أكتر من اليمنت هتسحب اللي عالوش
        // If not empty pop first element

        struct msqid_ds buf;
        // int message_num

        sleep(0.02);
        Process recievedProcess = recieveProcess(queueId);
        printf("id of recieved process: %d\n", recievedProcess.id);
        insert_by_priority(&recievedProcess);
        display_pqueue();
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;

        // printf("msg queue count: %d\n", message_num);

        while (message_num != 0)
        {
            sleep(0.01);
            recievedProcess = recieveProcess(queueId);
            printf("id of recieved process: %d\n", recievedProcess.id);
           
            insert_by_priority(&recievedProcess);
            rc = msgctl(queueId, IPC_STAT, &buf);
            message_num = buf.msg_qnum;
        }
        // display_pqueue(); 
        recievedProcess = popQueue();
        // printf("Run time of Popped process: %d\n", recievedProcess.runTime);
        recievedProcess.state = running;
        // fflush(stdout);
        // printf("startingTime= %d\n", getClk());
        // if (recievedProcess.state == running)
        // {
            // fflush(stdout);
            printf("process %d entered\n",recievedProcess.id);
            lastFlag = recievedProcess.flagLast;
            // printf("Last Flag: %d\n", lastFlag);
            char remain[10];
            // fflush(stdout);
            sprintf(remain, "%d", recievedProcess.runTime);
            // printf("remaining time = %d\n", recievedProcess.runTime);
            int pid = fork();
            // printf("fork pid: %d\n",pid);
            // fflush(stdout);
            if (pid == 0)
            {
                // fflush(stdout);
                printf("I am the fucking process");
                run("process", remain, NULL);
            }
        // }
    }
}

int main(int argc, char *argv[])
{
    initClk();
    create();
    if (atoi(argv[0])==3)
    {
        setKey(runTime);
        sjfAlgorithm();

    }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    sleep(4);
    // destroyClk(true);
}

