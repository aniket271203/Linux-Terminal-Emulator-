#include "headers.h"

int isHidden(const char *name)
{
    return name[0] == '.';
}

void convert(char *str, char *homeDir)
{
    strcat(homeDir, str + 1);
    strcpy(str, homeDir);
}

void print_colored(const char *name, const char *color)
{
    printf("%s%s%s\n", color, name, COLOR_RESET);
}

int compare_names(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int list(char *string, int flag)
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(string);

    if (dr == NULL)
    {
        perror("dr");
        return -1;
    }
    else
    {
        char *all_names[256]; // Array to collect all names
        int count = 0;

        while ((en = readdir(dr)) != NULL)
        {
            if (flag == 1)
            {
                if (isHidden(en->d_name))
                {
                    continue;
                }
            }

            all_names[count] = strdup(en->d_name);
            count++;
        }

        closedir(dr);

        // Sort the array
        qsort(all_names, count, sizeof(char *), compare_names);

        // Print the sorted names with appropriate colors
        for (int i = 0; i < count; i++)
        {
            struct stat file_stat;
            char fullpath[256];
            strcpy(fullpath, string);
            strcat(fullpath, "/");
            strcat(fullpath, all_names[i]);

            if (stat(fullpath, &file_stat) == -1)
            {
                perror("Error getting file status");
                continue;
            }

            if (S_ISDIR(file_stat.st_mode))
            {
                print_colored(all_names[i], COLOR_BLUE);
            }
            else if (access(fullpath, X_OK) == 0)
            {
                print_colored(all_names[i], COLOR_GREEN);
            }
            else
            {
                print_colored(all_names[i], COLOR_WHITE);
            }

            free(all_names[i]);
        }
    }
    return 0;
}

int list_details(char *string, int flag)
{
    DIR *dr;
    dr = opendir(string);
    struct dirent *en;
    if (dr == NULL)
    {
        perror("dr");
        return -1;
    }
    else
    {
        char *all_names[256]; // Array to collect all names
        int count = 0;

        while ((en = readdir(dr)) != NULL)
        {
            if (flag == 1)
            {
                if (isHidden(en->d_name))
                {
                    continue;
                }
            }

            all_names[count] = strdup(en->d_name);
            count++;
        }

        closedir(dr);

        // Sort the array
        qsort(all_names, count, sizeof(char *), compare_names);

        // Print the sorted details with appropriate colors
        for (int i = 0; i < count; i++)
        {
            char fullpath[256];
            strcpy(fullpath, string);
            strcat(fullpath, "/");
            strcat(fullpath, all_names[i]);

            struct stat file_stat;
            char file_permissions[11];

            if (lstat(fullpath, &file_stat) == 0)
            {
                // Format file permissions
                mode_t mode = file_stat.st_mode;
                snprintf(file_permissions, sizeof(file_permissions),
                         "%c%c%c%c%c%c%c%c%c%c",
                         (S_ISDIR(mode)) ? 'd' : '-',
                         (mode & S_IRUSR) ? 'r' : '-',
                         (mode & S_IWUSR) ? 'w' : '-',
                         (mode & S_IXUSR) ? 'x' : '-',
                         (mode & S_IRGRP) ? 'r' : '-',
                         (mode & S_IWGRP) ? 'w' : '-',
                         (mode & S_IXGRP) ? 'x' : '-',
                         (mode & S_IROTH) ? 'r' : '-',
                         (mode & S_IWOTH) ? 'w' : '-',
                         (mode & S_IXOTH) ? 'x' : '-');

                // Format date and time
                char date_string[100];
                strftime(date_string, sizeof(date_string), "%b %e %H:%M", localtime(&file_stat.st_mtime));

                // Determine the color based on type
                const char *color = S_ISDIR(file_stat.st_mode) ? COLOR_BLUE : (access(all_names[i], X_OK) == 0 ? COLOR_GREEN : COLOR_WHITE);

                printf("%s %2lu %-8s %-8s %8lld %s %s%s%s\n",
                       file_permissions,
                       (unsigned long)file_stat.st_nlink,
                       getpwuid(file_stat.st_uid)->pw_name,
                       getgrgid(file_stat.st_gid)->gr_name,
                       (long long)file_stat.st_size, date_string,
                       color, all_names[i], COLOR_RESET);
            }
            else
            {
                perror("lstat");
            }

            free(all_names[i]);
        }
    }
    return 0;
}

int peek(char *string, char *prevDir, char *homeDir)
{
    // printf("%s\n",string);
    char delimit[] = " \t\r\n\v\f ";
    char *str;
    char **flags=(char **)malloc(sizeof(char *) * 256);
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
        if (list(".", 1) == -1)
        {
            return -1;
        }
    }
    else if (n == 2)
    {
        if (strcmp(flags[1], "-") == 0)
            strcpy(flags[1], prevDir);
        if (flags[1][0] == '~')
            convert(flags[1], homeDir);
        if ((strcmp(flags[1], "-a") == 0))
        {
            if (list(".", 0) == -1)
            {
                return -1;
            }
        }
        else if (strcmp(flags[1], "-l") == 0)
        {
            if (list_details(".", 1) == -1)
            {
                return -1;
            }
        }
        else if ((strcmp(flags[1], "-al") == 0) || (strcmp(flags[1], "-la") == 0))
        {
            if (list_details(".", 0) == -1)
            {
                return -1;
            }
        }
        else
        {
            if (list(flags[1], 1) == -1)
            {
                return -1;
            }
        }
    }
    else if (n == 3)
    {
        if (strcmp(flags[2], "-") == 0)
            strcpy(flags[2], prevDir);
        if (flags[2][0] == '~')
            convert(flags[2], homeDir);
        if ((strcmp(flags[1], "-a") == 0))
        {
            if (list(flags[2], 0) == -1)
            {
                return -1;
            }
        }
        else if (strcmp(flags[1], "-l") == 0)
        {
            if (list_details(flags[2], 1) == -1)
            {
                return -1;
            }
        }
        else if ((strcmp(flags[1], "-al") == 0) || (strcmp(flags[1], "-la") == 0))
        {
            if (list_details(flags[2], 0) == -1)
            {
                return -1;
            }
        }
        else
        {
            printf("Invalid flag\n");
            return -1;
        }
    }
    else if (n == 4)
    {
        if (strcmp(flags[3], "-") == 0)
            strcpy(flags[3], prevDir);
        if (flags[3][0] == '~')
            convert(flags[3], homeDir);
        if ((strcmp(flags[1], "-a") == 0) && (strcmp(flags[2], "-l") == 0) || (strcmp(flags[2], "-a") == 0) && (strcmp(flags[1], "-l") == 0))
            if (list_details(flags[3], 0) == -1)
            {
                return -1;
            }
    }
    free(flags);
    return 0;
}
