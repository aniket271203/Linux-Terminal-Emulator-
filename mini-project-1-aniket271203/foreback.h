#ifndef __FOREBACK_H
#define __FOREBACK_H

typedef struct
{
    int pid;
    int status;
    int done;
    char command[20];
} BackgroundProcess;
void execute_foreground();
pid_t execute_background();
void check();

#endif