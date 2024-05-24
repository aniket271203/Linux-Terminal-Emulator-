#include "headers.h"

// 0 - directory
// 1 - files
// 2 - e output
// 3 - e and then warp into that directory
// 4 - e and then print the text from the file

void file_display(char *filename)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) == -1)
    {
        perror("stat");
    }

    if ((fileStat.st_mode & S_IRUSR) == 0)
    {
        fprintf(stderr, "No read permission for %s\n", filename);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("fopen");
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);
}

int f = 0;
int lookfor(char *directory, char *homeDir, int flag, char *original, char *prevDir)
{
    int count = 0;
    char exc_path[256] = "\0";
    char file[256] = "\0";
    DIR *dr;
    struct dirent *en;
    dr = opendir(homeDir);

    if (dr == NULL)
    {
        perror("dr");
        return -1;
    }
    else
    {
        while ((en = readdir(dr)) != NULL)
        {
            if (strcmp(en->d_name, ".") == 0 || strcmp(en->d_name, "..") == 0)
            {
                continue; // Skip current directory and parent directory entries
            }

            char fullpath[256];
            strcpy(fullpath, homeDir);
            strcat(fullpath, "/");
            strcat(fullpath, en->d_name);

            struct stat file_stat;
            if (stat(fullpath, &file_stat) == -1)
            {
                perror("Error getting file status");
                continue;
            }

            // Print the full path
            char path[256];
            strcpy(path, homeDir);
            strcat(path, "/");
            strcat(path, en->d_name);

            if (S_ISDIR(file_stat.st_mode))
            {

                if (flag != 1 && flag != 5)
                {
                    char modifiedpath[FILENAME_MAX];
                    int homeDirLength = strlen(original);
                    if (strcmp(en->d_name, directory) == 0)
                    {
                        f = 1;
                        if (count == 0)
                        {
                            strcpy(exc_path, path);
                        }
                        count++;
                        if (strlen(path) >= homeDirLength)
                        {
                            strcpy(modifiedpath, ".");
                            strcat(modifiedpath, path + homeDirLength);
                            print_colored(modifiedpath, COLOR_BLUE); // Directory path
                        }
                    }
                    // else if (strstr(en->d_name, directory) != NULL )
                    // {
                    //     f = 1;
                    //     if (strlen(path) >= homeDirLength)
                    //     {
                    //         strcpy(modifiedpath, ".");
                    //         strcat(modifiedpath, path + homeDirLength);
                    //         print_colored(modifiedpath, COLOR_BLUE); // Directory path
                    //     }
                    //     count++;
                    // }
                }
                char copy[256];
                strcpy(copy, homeDir);
                strcat(copy, "/");
                strcat(copy, en->d_name);
                lookfor(directory, copy, flag, original, prevDir);
            }
            else if (flag != 0 && flag != 4)
            {
                char modifiedpath[FILENAME_MAX];
                int homeDirLength = strlen(original);
                if (strstr(en->d_name, directory) != NULL)
                {
                    if (strcmp(en->d_name, directory) == 0 || en->d_name[strlen(directory)] == '.')
                    {
                        f = 1;
                        if (count == 0)
                        {
                            strcpy(file, path);
                            // printf("a");
                        }
                        if (strlen(path) >= homeDirLength)
                        {
                            strcpy(modifiedpath, ".");
                            strcat(modifiedpath, path + homeDirLength);
                            print_colored(modifiedpath, COLOR_WHITE); // File path
                        }
                        count++;
                    }
                    // else if (strstr(en->d_name, directory) != NULL )
                    // {
                    //     f = 1;
                    //     if (strlen(path) >= homeDirLength)
                    //     {
                    //         strcpy(modifiedpath, ".");
                    //         strcat(modifiedpath, path + homeDirLength);
                    //         print_colored(modifiedpath, COLOR_WHITE); // File path
                    //     }
                    //     count++;
                    // }
                }
            }
        }
        closedir(dr);
    }
    if (flag >= 3 && count == 1)
    {
        // printf("%s\n%s\n", exc_path, file);
        if (strcmp(file, "\0") == 0)
        {
            getcwd(prevDir, FILENAME_MAX);
            if (chdir(exc_path) == -1)
            {
                perror("chdir");
            }
        }
        else if (strcmp(exc_path, "\0") == 0)
        {
            // printf("anoc\n");
            file_display(file);
            printf("\n");
        }
    }
    return 0;
}

int seek(char *string, char *homeDir, char *prevDir)
{
    getcwd(homeDir, FILENAME_MAX);
    f = 0;
    char original[FILENAME_MAX];
    strcpy(original, homeDir);
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
    if (n == 2)
    {
        if (flags[1][0] == '.')
        {
            printf("invalid command\n");
            return -1;
        }
        lookfor(flags[1], homeDir, 2, original, prevDir);
    }
    else if (n == 3)
    {
        if (flags[1][0] == '-')
        {
            if (strcmp(flags[1], "-f") == 0)
            {
                if (lookfor(flags[2], homeDir, 1, original, prevDir) == -1)
                    return -1;
            }
            else if (strcmp(flags[1], "-d") == 0)
            {
                if (lookfor(flags[2], homeDir, 0, original, prevDir) == -1)
                    return -1;
            }
            else if (strcmp(flags[1], "-e") == 0)
            {
                if (lookfor(flags[2], homeDir, 3, original, prevDir) == -1)
                    return -1;
            }
            else
            {
                printf("invalid flag\n");
                return -1;
            }
        }
        else
        {
            strcat(homeDir, flags[2] + 1);
            if (lookfor(flags[1], homeDir, 2, original, prevDir) == -1)
                return -1;
        }
    }
    else if (n == 4)
    {
        strcat(homeDir, flags[3] + 1);
        if ((strcmp(flags[1], "-f") == 0 && strcmp(flags[2], "-d") == 0) || (strcmp(flags[2], "-f") == 0 && strcmp(flags[1], "-d") == 0))
        {
            printf("invalid flag\n");
            return -1;
        }
        else if (strcmp(flags[1], "-f") == 0)
        {
            if (lookfor(flags[2], homeDir, 1, original, prevDir) == -1)
                return -1;
        }
        else if (strcmp(flags[1], "-d") == 0)
        {
            if (lookfor(flags[2], homeDir, 0, original, prevDir) == -1)
                return -1;
        }
        else if (strcmp(flags[1], "-e") == 0)
        {
            if (lookfor(flags[2], homeDir, 3, original, prevDir) == -1)
                return -1;
        }
        else
        {
            printf("invalid flag\n");
            return -1;
        }
    }
    else if (n == 5)
    {
        strcat(homeDir, flags[4] + 1);
        if ((strcmp(flags[1], "-f") == 0 && strcmp(flags[2], "-d") == 0) || (strcmp(flags[2], "-f") == 0 && strcmp(flags[1], "-d") == 0))
        {
            printf("invalid flag\n");
            return -1;
        }
        else if ((strcmp(flags[1], "-d") == 0 && strcmp(flags[2], "-e") == 0) || (strcmp(flags[2], "-d") == 0 && strcmp(flags[1], "-e") == 0))
        {
            if (lookfor(flags[3], homeDir, 4, original, prevDir) == -1)
                return -1;
        }
        else if ((strcmp(flags[1], "-f") == 0 && strcmp(flags[2], "-e") == 0) || (strcmp(flags[2], "-f") == 0 && strcmp(flags[1], "-e") == 0))
        {
            if (lookfor(flags[3], homeDir, 5, original, prevDir) == -1)
                return -1;
        }
        else
        {
            printf("invalid flag\n");
            return -1;
        }
    }
    if (f == 0)
    {
        printf("No match found!\n");
        return 0;
    }
    free(flags);
}