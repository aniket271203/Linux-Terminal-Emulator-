#include "headers.h"

void prompt(char *homeDir,char* pro)
{
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");
    char directory[FILENAME_MAX];
    char modifiedDirectory[FILENAME_MAX];
    char hostname[_SC_HOST_NAME_MAX];
    char *username;
    username = (char *)malloc(20 * sizeof(char));
    gethostname(hostname, _SC_HOST_NAME_MAX);
    username = getlogin();
    getcwd(directory, FILENAME_MAX);
    int homeDirLength = strlen(homeDir);

    if (strlen(directory) >= homeDirLength)
    {
        strcpy(modifiedDirectory, "~");
        strcat(modifiedDirectory, directory + homeDirLength);
        printf("<%s%s%s@%s%s%s:%s%s%s%s%s%s>", COLOR_GREEN, username, COLOR_RESET, COLOR_GREEN, hostname, COLOR_RESET, COLOR_BLUE,modifiedDirectory,COLOR_RESET,COLOR_YELLOW,pro,COLOR_RESET);
    }
    else
    {
        printf("<%s%s%s@%s%s%s:%s%s%s%s%s%s>", COLOR_GREEN, username, COLOR_RESET, COLOR_GREEN, hostname, COLOR_RESET, COLOR_BLUE,directory,COLOR_RESET,COLOR_YELLOW,pro,COLOR_RESET);
    }
}
