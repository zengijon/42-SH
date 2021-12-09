//
// Created by jennie on 06/12/2021.
//

#include "builtins.h"

#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../struct/exec_struct.h"

char *final_path(char *name, char *current_path)
{
    char *path = calloc(1, strlen(name) + strlen(current_path) + 2); // null terminated
    path = strcat(path, current_path);
    if (current_path[strlen(current_path)] != '/')
        path = strcat(path, "/");
    path = strcat(path, name); // path abs
    return path;
}

size_t len_path(char *current_path)
{
    size_t nb = 0;
    char *token = strdup(current_path);
    if (token != NULL)
    {
        token = strtok(token, "/");
        nb = 1;
        while(token != NULL)
        {
            token = strtok(NULL, "/");
            nb++;
        }
    }
    free(token);
    return nb;
}

int is_valid_dir(char *path) // PB : clion enlever toujours le dernier dir donc le working directory est faux
{
    DIR *pDir = opendir(path);
    if (pDir == NULL)
    {
        printf ("Cannot open directory\n");
        return 0;
    }
    closedir (pDir);
    return 1;
}

int is_valid_symlink(char *path)
{
    struct stat p_statbuf;

    if (lstat(path, &p_statbuf) < 0) {  /* if error occured */
        perror("calling stat()");
        exit(1);  /* end progam here */
    }

    if (S_ISLNK(p_statbuf.st_mode) == 1)
        return 1;
    else
        return 0;
}

int is_valid_d_coma(char *arg, size_t nb_tok_path)
{
    size_t len = strlen(arg);
    size_t count = 0;
    size_t i = 0;
    if (len % 3 == 0)
    {
        while (i < len)
        {
            if (count >= nb_tok_path)
                errx(1, "sequence of ../ is too long");

            if (arg[i] == '.' && i + 1 < len && arg[i + 1] == '.')
            {
                i += 2;
                if (i <= len && arg[i] == '/')
                {
                    i++;
                    count++;
                }
                else
                {
                    if (i != len)
                        return 0;
                    return 1; // can finish without a /
                }
            }
            else
                return 0;
        }
        return 1;
    }
    return 0; // not a sequence of ../
}

char *find_old_path(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(e_x->v_l[i].name, name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

const char *get_cd_arg(char *arg, char *current_path, size_t nb_tok_path, struct exec_struct *e_x, int *need_pwd)
{
    char *path = final_path(arg, current_path);
    if (arg == NULL)
        return "/home";

    if (strcmp(arg, "-") == 0)
    {
        char *old_path = find_old_path(e_x, "OLDPWD");
        if (old_path == NULL)
            errx(1, "OLDPWD not inizialized");
        *need_pwd = 1;
        return old_path;
    }

    if (strcmp(arg, ".") == 0)
        return arg;

    if (strcmp(arg, "..") == 0)
        return arg;

    if (arg[0] == '.')
    {
        if (strlen(arg) > 1 && isalpha(arg[1]) != 0)
            return is_valid_dir(path) == 1 ? arg : NULL; // hidden dir

        return is_valid_d_coma(arg, nb_tok_path) == 1 ? arg : NULL; // sequence of ../
    }
    else
    {
        if (is_valid_symlink(path) == 1)
            return arg;
        return is_valid_dir(path) == 1 ? arg : NULL;
    }
}

int my_cd(char **argv, struct exec_struct *e_x)
{
    char s[2048] = { 0 };
    int need_pwd = 0;
    char *current_path = getcwd(s, 2048);

    if (current_path == NULL)
        errx(1, "getcwd failed");
    size_t len_current_path = len_path(current_path);
    const char *arg_cd = get_cd_arg(argv[1], current_path, len_current_path, e_x, &need_pwd);

    if (arg_cd == NULL)
        errx(1, "This is not a valid operator");
    assign_var("OLDPWD", current_path, e_x);
    if (chdir(arg_cd) == -1)
        errx(1, "Chdir failed");

    if (need_pwd == 1)
        printf("%s\n", getcwd(s, 2048));

    return 0;
}
