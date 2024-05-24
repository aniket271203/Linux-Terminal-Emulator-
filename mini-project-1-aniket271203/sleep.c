#include "headers.h"

int sleep_c(char *string, char *prompt)
{
    char delimit[] = " \t\r\n\v\f ";
    char *str;
    str = strtok(string, delimit);
    char *arg[256];
    int n = 0;
    while (str != NULL)
    {
        arg[n++] = str;
        str = strtok(NULL, delimit);
    }
    int len = strlen(arg[1]);
    int p = 0;
    int i = 0;
    while (p != len)
        i = i * 10 + (arg[1][p++] - '0');
    sleep(i);
    if (i > 2)
    {
        sprintf(prompt, " sleep : %ds", i);
    }
    printf("Sleep exited normally\n");
    return 0;
}
