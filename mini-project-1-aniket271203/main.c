#include "headers.h"

int main()
{
    // Keep accepting commands
    pastlist *list = (pastlist *)malloc(sizeof(pastlist));
    list->head = NULL;
    list->count = 0;
    char homeDir[FILENAME_MAX];
    char home[FILENAME_MAX];
    getcwd(home, FILENAME_MAX);
    char prevDir[FILENAME_MAX];
    init_list(list);
    int index;
    char *pro = (char *)malloc(sizeof(char) * 200);
    strcpy(pro, "\0");
    BackgroundProcess *PID = (BackgroundProcess *)malloc(sizeof(BackgroundProcess) * 200);
    while (1)
    {
        int fl = 0;
        strcpy(homeDir, home);
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(homeDir, pro);
        strcpy(pro, "\0");
        char input[4096];
        char *copy_input = (char *)malloc(sizeof(char) * 4096);
        char *in = (char *)malloc(sizeof(char) * 4096);
        strcpy(in,"\0");
        fgets(input, 4096, stdin);

        time_t original = time(NULL);

        strcpy(copy_input, input);

        char *str;
        char **string=(char **)malloc(sizeof(char *) * 256);
        char delimit[] = ";";
        int i = 0;
        str = strtok(input, delimit);
        while (str != NULL)
        {
            string[i] = str;
            i++;
            str = strtok(NULL, delimit);
        }

        for (int j = 0; j < i; j++)
        {
            char *str;
            char delimit[] = "&";
            char **command = (char **)malloc(sizeof(char *) * 256);
            int n = 0;
            str = strtok(string[j], delimit);
            while (str != NULL)
            {
                command[n] = str;
                n++;
                str = strtok(NULL, delimit);
            }

            for (int k = 0; k < n - 1; k++)
            {
                char delimit[] = " \t\r\n\v\f ";
                char *str;
                char **tokens = (char **)malloc(sizeof(char *) * 256);
                int n1 = 0;
                str = strtok(command[k], delimit);
                while (str != NULL)
                {
                    tokens[n1] = str;
                    n1++;
                    str = strtok(NULL, delimit);
                }
                tokens[n1] = NULL;
                index = execute_background(tokens, PID, index);
                free(tokens);
            }

            if (strcmp(command[n - 1], "\n") != 0)
            {
                // printf("a\n");
                strcpy(string[j], command[n - 1]);
            }
            else
            {
                // printf("acsa\n");
                i--;
            }
            free(command);
        }

        // i have i no. of arguments from the user as input of which some are arguments and others are the functions
        for (int j = 0; j < i; j++)
        {
            char *copy = (char *)malloc(sizeof(char) * 256);
            strcpy(copy, string[j]);
            char *str;
            str = strtok(string[j], " \t\r\n\v\f ");
            // printf("%s\n", str);
            strcat(in, str);
            if (strcmp(str, "warp") == 0)
            {
                strcpy(string[j], copy);
                int p = warp(copy, prevDir, homeDir);
            }
            else if (strcmp(str, "peek") == 0)
            {
                strcpy(string[j], copy);
                int p = peek(copy, prevDir, homeDir);
            }
            else if (strcmp(str, "proclore") == 0)
            {
                strcpy(string[j], copy);
                int p = proclore(copy, homeDir,PID,index);
            }
            else if (strcmp(str, "pastevents") == 0)
            {
                char *command = (char *)malloc(sizeof(char) * 256);
                command = pastevents(copy, list);
                if (command != NULL)
                {
                    string[i++] = command;
                    events_insert(list, command);
                }
                fl = 1;
                continue;
            }
            else if (strcmp(str, "sleep") == 0)
            {
                strcpy(string[j], copy);
                int p = sleep_c(copy, pro);
            }
            else if (strcmp(str, "echo") == 0)
            {
                system(copy);
            }
            else if (strcmp(str, "gedit") == 0)
            {
                system("gedit");
            }
            else if (strcmp(str, "seek") == 0)
            {
                strcpy(string[j], copy);
                int p = seek(copy, homeDir, prevDir);
            }
            else if (strcmp(str, "cd") == 0)
            {
                str = strtok(NULL, " \t\r\n\v\f ");
                if (str != NULL)
                {
                    getcwd(prevDir, FILENAME_MAX);
                    int status = chdir(str);
                    if (status == -1)
                    {
                        perror("cd");
                    }
                }
            }
            else if (strcmp(str, "exit") == 0)
            {
                printf("*** Exit Shell ***\n");
                events_insert(list, string[j]);
                // savelist(list);
                savelist(list, homeDir);
                return 0;
            }
            else
            {
                if (system(copy) == -1)
                {
                    printf("invalid command\n");
                }
            }

            strcat(in, ", ");
            free(copy);
        }

        if (fl != 1)
            events_insert(list, copy_input);

        for (int k = 0; k < index; k++)
        {
            if (PID[k].done == -1)
            {
                int status;
                pid_t wait_result;
                wait_result = waitpid(PID[k].pid, &status, WNOHANG);
                if (wait_result == -1)
                {
                    // Error occurred
                    perror("waitpid");
                }
                else if (wait_result == 0)
                {
                    // Background process is still running
                    continue;
                }
                else
                {
                    if (WIFEXITED(status))
                    {
                        printf("%s %d exited normally with status %d.\n", PID[k].command, PID[k].pid, WEXITSTATUS(status));
                        // PID[k].status = WEXITSTATUS(status);
                    }
                    else if (WIFSIGNALED(status))
                    {
                        printf("%s %d exited abnormally due to signal %d.\n", PID[k].command, PID[k].pid, WTERMSIG(status));
                        // PID[k].status = -1;
                    }
                    PID[k].done = 1;
                }
            }
        }
        free(copy_input);
        time_t after = time(NULL);
        int ti = after - original;
        if (ti > 2)
        {
            in[strlen(in)-2]='\0';
            sprintf(pro, " %s: %ds", in, ti);
        }
        free(in);
        free(string);
    }
    free(PID);
}
