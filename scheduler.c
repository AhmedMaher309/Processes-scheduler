#include "headers.h"
//Header is included in the priority queue
// TODO sort queu to arrival time

void SJF()
{
    int lastFlag = 0;
    fflush(stdout);
    printf("check this \n");
    while (!lastFlag)
    {
        int queueId = intMsgQueue(QKEY);
        Process recievedProcess = recieveProcess(queueId);

        //اللي يجيلك هتحطه في البرايورتي كيو وطول ما هي مش فاضية هتديقيو منها 
        //ولو فيها أكتر من اليمنت هتسحب اللي عالوش 
        //If not empty pop first element 
        recievedProcess.state = running;
        fflush(stdout);
        printf("startingTime= %d\n", getClk());
        if (recievedProcess.state == running)
        {
            fflush(stdout);
            printf("I entered \n");
            lastFlag = recievedProcess.flagLast;
            char remain[10];
            fflush(stdout);
            sprintf(remain, "%d", recievedProcess.remainingTime);
            int pid = fork();
            if (pid == 0)
            {
                fflush(stdout);
                printf("I am the fucking process");
                run("process", remain, NULL);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    initClk();
    // printf("This is the scheduler %s \n", argv[0]);
    // int queueId = intMsgQueue(QKEY);
    // Process rProcess = recieveProcess(queueId);
    // printf("arrival time = %d\n", rProcess.arrivalTime);
    // printf("run time = %d\n", rProcess.runTime);
    create();
    printf("%d",atoi(argv[0]));
    if (atoi(argv[0])==SJF)
    {
        setKey(runTime);
        SJF();

    }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    sleep(4);
    // lastFlag = 0
    // while(!lastFlag)
    //  queueId = intMsgQueue(Qkey)
    //
    //      Process rProcess = recieveProcess(queueId)
    //      rProcess.state = running;
    //      if (rProcess.state == running)
    //          lastFlag = rProcess.lastFlag;
    //          processPid= fork();
    //          if(processPid ==0)
    //          run("process",//send the result of the paramenters of the process)
    //

    // destroyClk(true);
}

/*
void SJF(int *arr){
    Process currentProccess;
    while(true){
        if(arr.front != arr.rear){
            if(arr.front->arrivalTime == (arr.front)+1->arrivalTime){
                if(arr.front->arrivalTime <= (arr.front)+1->arrivalTime)
                {
                    arr.pop(currentProccess);
                }
                else{
                    //take the other process
                }
            }
            else{
                arr.pop(currentProccess);
            }
        }
        if(arr.front == arr.rear){
            break;
        }

    }
}

*/
