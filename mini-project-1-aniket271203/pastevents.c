#include "headers.h"

void events_insert(pastlist *list, char *command)
{
    if (list->head != NULL && strcmp(list->head->command, command) == 0)
    {
        return;
    }
    if (list->count == 14)
    {
        node *tmp = list->head;
        int i = 0;
        while (i != 13)
        {
            tmp = tmp->next;
            i++;
        }
        free(tmp->next);
        tmp->next = NULL;
        list->count--;
    }
    node *new = (node *)malloc(sizeof(node));
    new->next = list->head;
    strcpy(new->command, command);
    list->head = new;
    list->count++;
    return;
}

void reverse(pastlist *list)
{
    node *tmp = list->head;
    node *prev = NULL;
    node *next;
    while (tmp != NULL)
    {
        next = tmp->next;
        tmp->next = prev;
        prev = tmp;
        tmp = next;
    }
    list->head = prev;
}
void pastevents_print(pastlist *list)
{
    reverse(list);
    node *tmp = list->head;
    while (tmp != NULL)
    {
        if (tmp->command[strlen(tmp->command) - 1] != '\n')
        {
            tmp->command[strlen(tmp->command) - 1] = '\n';
        }
        printf("%s", tmp->command);
        tmp = tmp->next;
    }
    reverse(list);
}

void pastevents_purge(pastlist *list)
{
    node *tmp = list->head;
    node *next;
    while (tmp != NULL)
    {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    list->head = NULL;
    list->count = 0;
}

char *pastevents_execute_index(pastlist *list, int index)
{
    node *tmp = list->head;
    int i = 1;
    while (i != index && tmp != NULL)
    {
        tmp = tmp->next;
        i++;
    }
    if (tmp == NULL)
    {
        printf("invalid index\n");
        return NULL;
    }
    return tmp->command;
}

void savelist(pastlist *list, char *homeDir)
{
    reverse(list);
    strcat(homeDir, "/list.txt");
    FILE *file = fopen(homeDir, "w");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }

    node *tmp = list->head;
    while (tmp != NULL)
    {
        size_t len = strlen(tmp->command);
        if (len > 0 && tmp->command[len - 1] == '\n')
        {
            tmp->command[len - 1] = '\0';
        }
        fprintf(file, "%s\n", tmp->command);
        tmp = tmp->next;
    }

    fclose(file);
    reverse(list);
}
void init_list(pastlist *list)
{
    FILE *file = fopen("list.txt", "r");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        events_insert(list, line);
    }
    fclose(file);
}

char *pastevents(char *string, pastlist *list)
{
    char delimit[] = " \t\r\n\v\f ";
    char *str;
    char **function = (char **)malloc(sizeof(char *) * 256);
    int n = 0;
    str = strtok(string, delimit);
    while (str != NULL)
    {
        function[n] = str;
        n++;
        str = strtok(NULL, delimit);
    }
    if (n == 1)
    {
        pastevents_print(list);
        free(function);
        return NULL;
    }
    else if (strcmp(function[1], "purge") == 0)
    {
        pastevents_purge(list);
        free(function);
        return NULL;
    }
    else if (strcmp(function[1], "execute") == 0)
    {
        int len = strlen(function[2]);
        int index;
        if (len == 0)
        {
            printf("invalid command\n");
            free(function);
            return NULL;
        }
        if (len == 1)
        {
            index = function[2][0] - '0';
        }
        else
        {
            index = (function[2][1] - '0') * 10 + (function[2][0] - '0');
        }
        if (index > 15)
        {
            printf("invalid index\n");
            free(function);
            return NULL;
        }
        char *command = (char *)malloc(sizeof(char) * 256);
        command = pastevents_execute_index(list, index);
        free(function);
        return command;
    }
    else
    {
        printf("invalid command");
        free(function);
        return NULL;
    }
}