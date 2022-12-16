//#include "headers.h"

/* Modify this file as needed*/
// int remainingtime;
// int currentClk = 0;

int main(int agrc, char *argv[])
{
    initClk();

    // TODO The process needs to get the remaining time from somewhere
    // remainingtime = ??;

    int remainingtime = atoi(argv[0]);
    int intialRemaining = remainingtime;
    while (remainingtime > 0)
    {
        initClk();
        int currentClk = getClk();
        // save the currentclk to be always equal to actual clk and reduce the remainingtime
        if (currentClk < getClk())
        {
            currentClk = getClk();
            
            remainingtime--;
        }
    }
    int waitingTime;
    destroyClk(false);

    return 0;
}
