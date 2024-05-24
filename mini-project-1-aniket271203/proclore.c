#include "headers.h"

int is_background_process(pid_t pid, BackgroundProcess PID[], int ind)
{
    // Get the PGID of the process
    pid_t pgid = getpgid(pid);

    // Get the PGID of the terminal's foreground process group
    pid_t foreground_pgid = tcgetpgrp(STDIN_FILENO);

    // Compare the PGIDs to determine if it's a background process
    for (int i = 0; i < ind; i++)
    {
        if (pid == PID[i].pid)
        {
            return 1;
        }
    }
    return (pgid != foreground_pgid);
}

int print_process_info(pid_t pid, char *homeDir, BackgroundProcess PID[], int ind)
{
    char proc_path[50];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *file = fopen(proc_path, "r");
    if (file == NULL)
    {
        perror("Error opening proc file");
        return -1;
    }

    printf("PID: %d\n", pid);
    char line[256];
    char c;
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "State:", 6) == 0)
        {
            char status_symbol = is_background_process(pid, PID, ind) ? ' ' : '+';

            printf("Process Status: %c%c\n", line[7], status_symbol);
        }
        else if (strncmp(line, "PPid:", 5) == 0)
        {
            printf("Process Group: %s", line + 6);
        }
        else if (strncmp(line, "VmSize:", 7) == 0)
        {
            printf("Virtual Memory: %s", line + 8);
        }
    }
    fclose(file);

    char exe_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
        char modifiedpath[FILENAME_MAX];
        int homeDirLength = strlen(homeDir);
        if (strlen(exe_path) >= homeDirLength)
        {
            strcpy(modifiedpath, "~");
            strcat(modifiedpath, exe_path + homeDirLength);
            printf("Executable Path: %s\n", modifiedpath);
        }
        else
            printf("Executable Path: %s\n", exe_path);
    }

    return 0;
}

int proclore(char *string, char *homeDir, BackgroundProcess PID[], int ind)
{
    char delimit[] = " \t\r\n\v\f ";
    char *str;
    char **flags = (char **)malloc(sizeof(char *) * 256);
    int n = 0;
    str = strtok(string, delimit);
    while (str != NULL)
    {
        flags[n] = str;
        n++;
        str = strtok(NULL, delimit);
    }

    if (n == 1)
    {
        // Print information about the shell's own process
        if (print_process_info(getpid(), homeDir, PID, ind) == -1)
        {
            return -1;
        }
    }
    else if (n == 2)
    {
        int index = atoi(flags[1]);
        if (print_process_info(index, homeDir, PID, ind) == -1)
        {
            return -1;
        }
    }
    else
    {
        printf("Invalid number of arguments.\n");
        return -1;
    }
    free(flags);
    return 0;
}
