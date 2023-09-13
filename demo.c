#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char ** argv)
{
    printf("\n%s\n", argv[0]);
    
    //my_pid = fork();

    int pid_from_fork = -1;
    int my_pid = 0;

    pid_from_fork = fork();
    switch(pid_from_fork)
    {
        case(0):
            my_pid = getpid();
            printf("\nHi from the child: my_pid = %d\n", my_pid);
            break;
        case(-1):
            printf("\nHi from the parent (FORK Failed)");
            break;
        default:
            my_pid = getpid();
            printf("\nHi from the parent (my_pid = %d, child pid = %d)\n)", my_pid, pid_from_fork);
            break;
    }

    return 0;
}