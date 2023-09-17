// 9/12/23 ~ 9/14/23
// Forks a child process and the child process does the "ls" command.
// the parent process waits for child to finish and reports termination status
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    printf("\n%s\n", argv[0]);  //* prints name of program
    
      //my_pid = fork();

    int   pid_from_fork = -1;
    int   my_pid        = 0;
    int   err           = 0;
    pid_t wait_pid      = 0;

    pid_from_fork = fork();     //* creates child process. Now there is parent and child running

    switch(pid_from_fork)
    {
        case(0):                //* for child process
            my_pid = getpid();
            printf("\nHi from the child: my_pid = %d\n", my_pid);
            err = execl("/usr/bin/ls","usr/bin/ls","/","-all",(char *)NULL);  //* the first set is the path to the command you want to execute
                                                                                //* the second set is the argv[0] value. argv[0] is program's own name
            //err = execl("/bin/ls", "ls", "-all", "/shared_folder", (char *)NULL); //* ls everything in shared_folder
            break;

        case(-1):               //* fork failed
            printf("\nHi from the parent (FORK Failed)");
            break;

        default:                //* parent process
            my_pid    = getpid();
            int state = 0;
            wait_pid  = wait(&state);
            //printf("\nFrom parent(child PID = %d): mmy_pid = %d\n", pid_from_fork,my_pid);
            printf("\nHi from the parent (my_pid = %d, child pid = %d)\n)", my_pid, pid_from_fork);
            printf("\nChild process finished: wait_pid = %d, state = %d\n", wait_pid, state);
            break;
    }

    return 0;
}



