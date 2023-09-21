#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define max_input 1024  //* max characters

int main() {
    char input[max_input];

    while (1) {
        printf("Shell> ");
        fgets(input, max_input, stdin);         //* fgets = reads input from file stream
                                                //* all together waits for user input
        
        input[strcspn(input, "\n")] = '\0';     //* Remove the trailing newline character

        
        char *token;                 //* decalres pointer that will store arguemnts from fgets
        char *tokens[max_input];
        int token_count = 0;         //* will track the amount of arguments

        token = strtok(input, " ");  //* will split tokens whereever theres a space
        while (token != NULL) {     //* keep looping as long as it finds tokens
            tokens[token_count++] = token;
            token                 = strtok(NULL, " ");  //* gets next token from input 
        }
        
        // Fork a child process
        pid_t pid = fork();  //? idk why both

        switch (pid) {
            case -1:            //* Fork failed
                printf("\nHi from the parent (FORK Failed)");
                break;

            case 0:             //* Child process
                {
                    int my_pid = getpid();
                    printf("Hi from the child: my_pid = %d\n", my_pid);

                      // Execute the command using execl
                    if (execvp(tokens[0], tokens) == -1) {
                        perror("Exec failed");
                        exit(1);
                    }

                    break;
                }

            default:            //* Parent process
                {
                    int my_pid = getpid();
                    int state;
                    pid_t wait_pid = wait(&state);

                    printf("Hi from the parent (my_pid = %d, child pid = %d)\n", my_pid, pid);
                    printf("Child process finished: wait_pid = %d, state = %d\n", wait_pid, state);
                    break;
                }
        }
    }

    return 0;
}
