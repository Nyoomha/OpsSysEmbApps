#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
// 9/12/23
int main(int argc, char ** argv)
{
    printf("\n%s\n", argv[0]);
    
    //my_pid = fork();

    int pid_from_fork = -1;
    int my_pid = 0;
    int err = 0;
    pid_t wait_pid = 0;

    pid_from_fork = fork();

    switch(pid_from_fork)
    {
        case(0):
            my_pid = getpid();
            printf("\nHi from the child: my_pid = %d\n", my_pid);
            err = execl("/usr/bin/ls","usr/bin/ls","/","-all",(char *)NULL);
            break;
        case(-1):
            printf("\nHi from the parent (FORK Failed)");
            break;
        default:
            my_pid = getpid();
            int state = 0;
            wait_pid = wait(&state);
            //printf("\nFrom parent(child PID = %d): mmy_pid = %d\n", pid_from_fork,my_pid);
            printf("\nHi from the parent (my_pid = %d, child pid = %d)\n)", my_pid, pid_from_fork);
            printf("\nChild process finished: wait_pid = %d, state = %d\n", wait_pid, state);
            break;
    }

    return 0;
}



