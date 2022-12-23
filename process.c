#include "headers.h"

/* Modify this file as needed*/
// int remainingtime;
// int currentClk = 0;

int main(int agrc, char *argv[])
{
    initClk();
    printf("in process.c \n");
    // TODO The process needs to get the remaining time from somewhere
    // remainingtime = ??;

    int runTime = atoi(argv[0]);
    printf("the runtime of the process %d \n", runTime);
    int intialRunTime = runTime;
    while (runTime > 0)
    {

        runTime--;
        sleep(1);
    }
    printf("Process done\n");
    destroyClk(false);
    return 0;
}
