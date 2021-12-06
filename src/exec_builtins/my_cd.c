//
// Created by jennie on 06/12/2021.
//

#include "my_cd.h"

#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <err.h>
#include <stddef.h>

#include "../struct/exec_struct.h"

size_t strlen_path(char *current_path)
{
    size_t nb = 0;
    char *token = current_path;
    if (token != NULL)
    {
        token = strtok(current_path, "/");
        nb = 1;
        while(token != NULL)
        {
            token = strtok(NULL, "/");
            nb++;
        }
    }
    return nb;
}

int is_valid_dir(char *name, char *current_path)
{
    char *path = calloc(1, strlen(name) + strlen(current_path) + 2); // null terminated
    path = strcat(path, current_path);
    path = strcat(path, "/");
    path = strcat(path, name);

    //struct dirent *pDirent;
    DIR *pDir;

    pDir = opendir (path);
    if (pDir == NULL) {
        printf ("Cannot open directory\n");
        return 0;
    }
    closedir (pDir);
    return pDir == NULL ? 0 : 1;
}

int is_valid_d_coma(char *arg, size_t nb_tok_path)// reste ca a faire
{
    size_t len = strlen(arg);
    size_t count = 0;
    size_t i = 0;
    if (len % 3 == 0)
    {
        while (i + 2 < len)
        {
            if (count >= nb_tok_path)
                errx(1, "sequence of ../ is too long");
            if (arg[i] == '.')
            {
                i++;
                if (arg[i] == '.')
                {
                    i++;
                    if (arg[i] == '/')
                        i++;
                    else
                        return 1;
                }
                count++;
            }
            else
                return 0;
        }
    }
    return 1;
}

const char *get_cd_arg(char *arg, char *current_path, int need_pwd, size_t nb_tok_path)
{
    if (arg == NULL) ///--- Pour aller au HOME
        return "~";

    if (strcmp(arg, "-") == 0) ///--- Operand special
    {
        need_pwd = 1;
        return "$OLDPWD";
    }

    if (strcmp(arg, ".") == 0)
        return arg;

    if (strcmp(arg, "..") == 0)
        return arg;

    if (arg[0] == '.')
    {
        if (isalpha(arg[1]) != 0) ///--- Possiblement un dossier cache
            return is_valid_dir(arg, current_path) == 1 ? arg : NULL;

        return is_valid_d_coma(arg, nb_tok_path) == 1 ? arg : NULL;
    }
    else
        return is_valid_dir(arg, current_path) == 1 ? arg : NULL;
}

int my_cd(char *arg, struct exec_struct *e_x) // changer le oldpwd avant le "cd"
{
    char s[2048];
    int need_pwd = 0;
    char *current_path = getcwd(s, 2048);
    if (current_path == NULL)
        errx(1, "getcwd failed");
    size_t len_current_path = strlen_path(current_path);
    const char *arg_cd = get_cd_arg(arg, current_path, need_pwd, len_current_path);
    if (arg_cd == NULL)
        errx(1, "This is not a valid operator");

    printf("\n\n---===%s\n", current_path); // a enlever

    if (need_pwd == 1);
    /// il faut executer = cd "$OLDPWD" && pwd

    if (chdir(arg_cd) == -1)
        errx(1, "Chdir failed");

    printf("---%s\n", getcwd(s, 2048)); // a enlever

    return 0; // finish
}
