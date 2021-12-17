//
// Created by jennie on 15/12/2021.
//
#define _X_OPEN_SOURCE 700


#include "for_pwd.h"
#include <string.h>
#include "../memory/hmalloc.h"
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
//extern char *environ;

//int is_valid_dir(char *path)
//{
//    DIR *pDir = opendir(path);
//    if (pDir == NULL)
//        return 0;
//    closedir (pDir);
//    return 1;
//}

int nb_dirr(char *dir_path)
{
    int res = 0;
    for (size_t i = 1; i < strlen(dir_path); ++i)
    {
        if (dir_path[i] == '/')
            res++;
    }
    return res;
}

void concat(char *res, char *token)
{
    if (token != NULL)
    {
        if (token[0] != '/')
            strcat(res, "/");
        strcat(res, token);
    }
}

char *good_path(int indic, char **argv, char *pwd_env)
{
    //int nb_dir = nb_dirr(argv[indic]);
    char *res = hcalloc(1, strlen(pwd_env) + strlen(argv[indic]) + 25);
    memccpy(res, pwd_env, '\0',strlen(pwd_env));
    char *token = strtok(argv[indic], "/");
    while (token != NULL)
    {
        int is_file = 0;
        for (size_t i = 0; i < strlen(token); ++i)
            if (token[i] == '.')
            {
                is_file = 1;
                break;
            }
        if (is_file == 1)
            break;
        concat(res, token);
        token = strtok(NULL, "/");
    }
    return res;
}

char *get_current_wd(void)
{
    char *res = hcalloc(2049, 1);
    int size = 2048;
    while (getcwd(res, size) == NULL)
    {
        res = hrealloc(res, size * 2 + 1);
        size *= 2;
    }
    return res;
}

char *get_path(int indic, char **argv, int is_not_file)
{
    char *res;
    char *temp = getenv("PWD");

    if (temp == NULL)
        temp = get_current_wd();

    if (indic != -1 && is_not_file == 0)
        res = good_path(indic, argv, temp);
    else
    {
        res = hcalloc(1, strlen(temp) + 5);
        memccpy(res, temp, '\0', strlen(temp));
    }
    return res;
}
