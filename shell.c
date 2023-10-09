#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h> 

#define max_input 1024  //* max characters


void help();
void parseCmd(char* parsed);
void init_shell();

int main() {
    char input[max_input];
    init_shell();

    while (1) {
        printf("Shell> ");
        fgets(input, max_input, stdin);         //* fgets = reads input from file stream
                                                //* all together waits for user input

        input[strcspn(input, "\n")] = '\0';     //* Remove the trailing newline character


        char *token;                 // decalres pointer that will store arguemnts from fgets
        char *tokens[max_input];
        int token_count = 0;         // will track the amount of arguments
        pid_t pid = 0;

        token = strtok(input, " ");  // will split tokens whereever theres a space

        while (token != NULL) {     //* keep looping as long as it finds tokens
            tokens[token_count++] = token;
            parseCmd(token);
            token = strtok(NULL, " ");  //* gets next token from input 
        }


        // Fork a child process
        pid = fork();

        switch (pid) {
            case -1:            //* Fork failed
                printf("\n(FORK Failed)");
                break;

            case 0:             //* Child process
                {
                    int my_pid = getpid();
                    //printf("my_pid = %d\n", my_pid);

                    // Execute the command using execl
                    if (execv("/usr/bin/ls", tokens) == -1) {
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
                    break;
                }
        }
    }

    return 0;
}

void help() //function for shell help command
{
    puts(
        "\nSHELL HELP"
        "\nList of available commands:"
        "\n-exit   -exit shell"
        "\n-help   -list commands"
        "\n-date   -display current date"
        "\n-ls     -display current directory path"
        "\n-cd     -display or change current directory"
        );
}

void parseCmd(char* parsed) //checks built in cmds for shell
{
    char* listCmds[4];
    int tok;
    listCmds[0] = "exit";
    listCmds[1] = "help";
    listCmds[2] = "date";
    listCmds[3] = "cd";

    for(int i = 0; i < 3; i++)
    {
        if(strcmp(parsed,listCmds[i]) == 0)
            tok = i;
    }

    switch(tok)
    {
        case 0:
            exit(0);
            break;

        case 1:
            help();
            break;

        case 2:
            printf(__DATE__"\n");
            break;

        case 3:
            if (parsed[1] != '\0') {
                if (chdir(parsed + 1) == 0) {

                } 
                else {
                    perror("chdir");
                }
            } else {
                printf("cd <directory>\n");
            }
            break;


        default:
            break; 
    }
}

void init_shell()
{    printf("\n\n\n\n**"
        "**");
    printf("\n\n\n\tNO NAME SHELL");
    printf("\n\n\t\t%s", "\u263A");
    printf("\n\n\n\n**"
        "**\n");
}