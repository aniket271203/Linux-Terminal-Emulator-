#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

typedef struct node
{
    char command[30];
    struct node *next;
} node;

typedef struct pastlist
{
    node *head;
    int count;
} pastlist;

void events_insert();
char* pastevents(char *string, pastlist *list);
void savelist();
void init_list();

#endif