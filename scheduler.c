#include "headers.h"
#include "Process.h"

//TODO sort queu to arrival time

int main(int argc, char *argv[])
{
    initClk();

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    destroyClk(true);
}



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



