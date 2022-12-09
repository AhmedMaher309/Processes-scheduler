#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int currentClk = 0;


int main(int agrc, char *argv[])
{
    initClk();

    //TODO The process needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        initClk();
        remainingtime = atoi(argv[1]);

        currentClk = getClk();
        while (remainingtime > 0)
        {

            // save the currentclk to be always equal to actual clk and reduce the remainingtime
            if (currentClk < getClk())
            {
                currentClk = getClk();
                remainingtime--;
            }
        }
    }

    destroyClk(false);

    return 0;
}



