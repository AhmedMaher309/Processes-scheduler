
#include "headers.h"

/* Modify this file as needed*/
// int remainingtime;
// int currentClk = 0;

int main(int agrc, char *argv[])
{
    initClk();
    // printf("in process.c \n");
    // TODO The process needs to get the remaining time from somewhere
    // remainingtime = ??;

    int runTime = atoi(argv[0]);
    printf("runtime process.c %d\n",runTime);
    printf("the runtime of the process %d \n", runTime);
    int intialRunTime = runTime;
    while (runTime > 0)
    {

        runTime--;
        sleep(1);
    }
    printf("Current Time of finsihed process is %d \n", getClk());
    printf("Process done\n");
    // sleep(0.01);
    destroyClk(false);
    return 0;
}

