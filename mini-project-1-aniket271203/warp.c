#include "headers.h"

int home(char *prevDir, char *homeDir)
{
    char s[FILENAME_MAX];
    getcwd(prevDir, FILENAME_MAX);

    if (chdir(homeDir) != 0)
    {
        perror("chdir");
        return -1;
    }

    printf("%s\n", getcwd(s, 100));
    return 0;
}

int warp(char *string, char *prevDir, char *homeDir)
{
    // printf("%s\n",homeDir);
    char delimit[] = " \t\r\n\v\f ";
    char *str;
    char **arguments=(char **)malloc(sizeof(char *) * 256);
    int n = 0;
    str = strtok(string, delimit);
    char s[FILENAME_MAX];

    while (str != NULL)
    {
        arguments[n] = str;
        n++;
        str = strtok(NULL, delimit);
    }

    if (n == 1)
    {
        if (home(prevDir, homeDir) == -1)
        {
            return -1;
        }
    }
    char home_copy[FILENAME_MAX];
    for (int i = 1; i < n; i++)
    {
        strcpy(home_copy, homeDir);
        if (strcmp(arguments[i], "~") == 0)
        {
            if (home(prevDir, home_copy) == -1)
            {
                return -1;
            }
        }

        else if (strcmp(arguments[i], ".") == 0)
        {
            printf("%s\n", getcwd(s, 100));
        }

        else if (strcmp(arguments[i], "..") == 0)
        {
            getcwd(prevDir, FILENAME_MAX);
            if (chdir("..") != 0)
            {
                perror("chdir");
                return -1;
            }
            printf("%s\n", getcwd(s, 100));
        }

        else if (strcmp(arguments[i], "-") == 0)
        {
            if (prevDir == NULL)
            {
                printf("OLDPWD not set");
                return -1;
            }
            char current[FILENAME_MAX];
            getcwd(current, FILENAME_MAX);
            if (chdir(prevDir) != 0)
            {
                perror("chdir");
                return -1;
            }
            strcpy(prevDir,current);
            printf("%s\n", getcwd(s, 100));
        }

        else
        {
            if (arguments[i][0] == '~')
            {
                char *str = strtok(arguments[i], "/");
                while (str != NULL)
                {
                    arguments[i] = str;
                    str = strtok(NULL, "/");
                }
                strcat(home_copy, "/");
                arguments[i] = strcat(home_copy, arguments[i]);
            }
            getcwd(prevDir, FILENAME_MAX);
            if (chdir(arguments[i]) != 0)
            {
                perror("chdir");
                return -1;
            }
            printf("%s\n", getcwd(s, 100));
        }
    }
    free(arguments);
    return 0;
}
