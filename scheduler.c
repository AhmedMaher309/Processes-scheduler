#include "PriorityQueue.h"
#include "CircQueue.h"
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
    signal(SIGCHLD, handler);
    while (!lastFlag)
    {
        fflush(stdout);
        // x = getClk();
        sleep(1);
        x = getClk();
        int queueId = intMsgQueue(QKEY);
        struct msqid_ds buf;
        sleep(0.02);
        Process recievedProcess = recieveProcess(queueId, &rState);
        if (rState != -1)
        {
            printf("id of recieved process: %d\n", recievedProcess.id);
            insert_by_priority(&recievedProcess);
        }
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
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
            recievedProcess = popQueue();
            printf("rState = %d \n", rState);
            display_pqueue();
            recievedProcess.state = running;
            int currentProcess = recievedProcess.id;
            printf("process %d entered\n", recievedProcess.id);
            lastFlag = recievedProcess.flagLast;
            char remain[10];
            sprintf(remain, "%d", recievedProcess.runTime);
            runningFlag = 1;
            int processPid = fork();
            fflush(stdout);
            if (processPid == 0)
            {
                printf("This is the child \n");
                printf("This is great \n");
                run("process", remain, NULL);
            }
            int stat_loc;
            processPid = wait(&stat_loc);
            fflush(stdout);
            if(!(stat_loc & 0x00FF)){
                printf("%d has exited successfully\n", currentProcess);
            }

        }
    }
}


void rrAlgorithm(int quantum)
{
    signal(SIGCHLD, handler);
    int lastFlag=0; 
    int rc,x,rState,qState;
    int aProcessFinished=0;
    Process finishedProcess, recievedProcess;
    while (!lastFlag || runningFlag)
    {
        sleep(1);
        x=getClk();
        int queueId= intMsgQueue(QKEY);
        // Process recievedProcess=recieveProcess(queueId, &rState);
        // if (rState!=-1)
        // {
        //     //todo: insert in the circular queue
        //     qState=enQueue(&recievedProcess);
        // }


        //If there are multiple processes in the queue it will enter the while loop
        //Else it will work on the already recievedProcess above
        
        
        //while loop to recieve
        struct msqid_ds buf;
        rc = msgctl(queueId, IPC_STAT, &buf);
        int message_num = buf.msg_qnum;
        while (message_num != 0)
        {
            sleep(0.01);
            recievedProcess = recieveProcess(queueId, &rState);
            printf("id of recieved process: %d\n", recievedProcess.id);
            if (rState != -1)
            {
                //todo: insert in the circular queue
                qState=enQueue(&recievedProcess);
                rc = msgctl(queueId, IPC_STAT, &buf);
                message_num = buf.msg_qnum;
            }
        }
        if (aProcessFinished)
        {
            qState=enQueue(&finishedProcess);
            aProcessFinished=0;
        }

        //The size of circ queue must be size of the processes
    
        //while loop in the scheduler to compare quantum with current time remainder
        //then decide whether to 

        //Enqueue all processes in the circular queue (the rear will move)
        //until the msg queue is empty
        if (!isEmptyQueue() && !runningFlag)
        {
            finishedProcess=deQueue();
            finishedProcess.state=running;
            lastFlag=finishedProcess.flagLast;
            char remaining[10];
            
            runningFlag=1;            
              /*
                if rt =< quantum -> send remainingTime and don't put it back in the queue
                if rt > quantum -> mark it as returning back to the queue
            */
            if (finishedProcess.remainingTime>quantum) aProcessFinished=1;           
                //This will tell us that the process is returning again to the queue
    
            sprintf(remaining,"%d",finishedProcess.remainingTime);
          
            if (finishedProcess.forkId==0)
            {
                int pid=fork();
                if (pid==0) run("process",remaining,NULL);
                else 
                    finishedProcess.forkId=pid;              
            }
            else
            {
                kill(finishedProcess.forkId,SIGCONT);
            }
            
            x=getClk()
            while ( (getClk()-x) != quantum)
            {
                sleep(1);
                finishedProcess.remainingTime-=1;
                printf("Current Time is %d\n", x);
            }

            sleep(0.01);
            kill(finishedProcess.forkId,SIGSTOP);

        }
        
    }
    

    /*

    while (not all processes finished)
     
        check if a new process arrived

        check the remaining of the process
        if remainingTime < quantum -> make remainingTime=0 and exit it 
        else

            if not forked (i.e if not in the pidTracker) -> fork, else sigcont to this id
            stop the rest
            run this process 
            remainingTime-=quantum  
            stop 
            
            check if another one arrived 
            put at the end of the queue
    
    */

   int lastFlag=0;
   if (!isEmpty())
   

}


    

    /*
    while (not all processes finished)
     
        check if a new process arrived
        check the remaining of the process
        if remainingTime < quantum -> make remainingTime=0 and exit it 
        else

            if not forked (i.e if not in the pidTracker) -> fork, else sigcont to this id
            stop the rest
            run this process 
            remainingTime-=quantum  
            stop 
            check if another one arrived 
            put at the end of the queue
    
    */  
}



int main(int argc, char *argv[])
{
    initClk();
    create();
    printf("%d is my Parent \n", getppid());
    int chosenAlgorithm=atoi(argv[0]);

    if (chosenAlgorithm == SJF)
    {
        setKey(runTime);
        sjfAlgorithm();
    }
    else if (chosenAlgorithm==RR)
    {
        rrAlgorithm(atoi(argv[1]));
    }

    // TODO: implement the scheduler.
    // TODO: upon termination release the clock resources.
    //sleep(4);
    // destroyClk(true);
    printf("%d is my Parent \n", getppid());
    kill(getppid(), SIGINT);
    return 0;
}
