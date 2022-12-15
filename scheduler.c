#include "headers.h"
#include "Process.h"

// TODO sort queu to arrival time

int main(int argc, char *argv[])
{
    initClk();
    printf("This is the scheduler %s \n", argv[0]);
    int queueId = intMsgQueue(QKEY);
    Process rProcess = recieveProcess(queueId);
    printf("arrival time = %d\n", rProcess.arrivalTime);
    printf("run time = %d\n", rProcess.runTime);
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
