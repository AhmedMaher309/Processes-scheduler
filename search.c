#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>

void handler(int signum);
int x;
int pids[2];

void search(int array[], int x, int *position, int start, int end)
{

    for (int i = start; i < end; i++)
    {
        if (x == array[i])
        {

            *position = i;
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    // signal(SIGCHLD, handler);
    int size = argc - 2;
    int middle = ceil(size / 2);
    x = atoi(argv[1]);
    int array[size];

    int pid, stat_loc;

    int position1 = -1;
    int position2 = -1;
    int *ptr1 = &position1;
    int *ptr2 = &position2;

    //------------------- create array of values -------------------------------//

    for (int i = 0; i < size; i++)
    {
        array[i] = atoi(argv[i + 2]);
    }

    printf("I am the parent, PID = %d \n", getpid());
     
    //------------------- process both children & parent -------------------------------//

    for (int i = 0; i < 2; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("error forking \n");
        }
        else if (pids[i] == 0) // children
        {
            if (i == 0) // first child
            {
                printf("I am the first child, PID = %d, PPID = %d \n", getpid(), getppid());
                search(array, x, ptr1, 0, middle);
                if (position1 != -1)
                {
                    kill(getppid(), SIGUSR1);
                    exit(position1);
                }
                else
                {
                    sleep(3);
                    printf("child 1 terminates \n");
                    exit(0);
                }
            }
            else // second child
            {
                printf("I am the second child, PID = %d, PPID = %d \n", getpid(), getppid());
                search(array, x, ptr2, middle, size);
                if (position2 != -1)
                {
                    kill(getppid(), SIGUSR1);
                    exit(position2);
                }
                else
                {
                    sleep(3);
                    printf("child 2 terminates \n");
                    exit(0);
                }
            }
        }
    }

    // parent
    // printf("sleep \n");
    sleep(5);
    //  printf("end \n");
    for (int i = 0; i < 2; i++)
    {
        pid = wait(&stat_loc);
    }

    printf("Value Not Found \n");

    return 0;
}

//------------------------- HANDLER -------------------------------//

void handler(int signum)
{
    int pid, stat_loc, child;
    // printf("Child 1(or 2): Value X found at position Y");
    pid = wait(&stat_loc);
    if (pid == pids[0])
    {
        child = 1;
    }
    else
    {
        child = 2;
    }

    if (!(stat_loc & 0x00FF))
    {
        printf("\nChild %d : Value %d found at position %d\n", child, x, stat_loc >> 8);
        kill(pids[0], SIGKILL);
        kill(pids[1], SIGKILL);
        kill(getpid(), SIGKILL);
    };
}

// Q1 : handler will be excuted by parent only as it is sent to parent ID & the children exit

// Q2 : it is not possible as the handler will be called only when signal SIGUSER1 caught
// which will not be sent unless one of children find the value and sent its position as exit code to it