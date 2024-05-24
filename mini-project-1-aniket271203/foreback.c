#include "headers.h"
#define MAX_COMMAND_LENGTH 1024
#define MAX_TOKENS 20

void execute_foreground(char **tokens)
{
    pid_t child_pid;
    int status;
    child_pid = fork();
    if (child_pid == 0)
    {
        setsid();
        // Child process
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(1);
    }
    else if (child_pid > 0)
    {
        // Parent process
        waitpid(child_pid, &status, 0);
    }
    else
    {
        // Forking error
        perror("fork");
    }
}
int execute_background(char **tokens, BackgroundProcess id[], int index)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == 0)
    {
        // Child process
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(1);
    }
    else if (child_pid > 0)
    {
        // Parent process
        printf("%d\n",child_pid);
        id[index].pid = child_pid;
        id[index].status = -1; // Initialize status to -1 (not determined yet)
        id[index].done=-1;
        strcpy(id[index].command,tokens[0]);
        return index + 1; // Increment the index for the next background process
    }
    else
    {
        // Forking error
        perror("fork");
        return index; // Return the same index in case of an error
    }
}
