//
// Created by jennie on 06/12/2021.
//

#include "builtins.h"

#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../memory/hmalloc.h"

char *final_path(char *name, char *current_path)
{
    char *path = hcalloc(1, strlen(name) + strlen(current_path) + 18);
    path = strcat(path, current_path);
    if (current_path[strlen(current_path) - 1] != '/')
        path = strcat(path, "/");
    path = strcat(path, name);
    return path;
}

int is_valid_dir(char *path)
{
    DIR *pDir = opendir(path);
    if (pDir == NULL)
        return 0;

    closedir (pDir);
    return 1;
}

char *find_e_x(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(e_x->v_l[i].name, name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

const char *get_cd_arg(char *arg, char *current_path, struct exec_struct *e_x, int *need_pwd)

{
    if (arg == NULL || strcmp(arg, ";") == 0)
        return find_e_x(e_x, "HOME");

    char *path = final_path(arg, current_path);

    if (strcmp(arg, "-") == 0)
    {
        char *old_path = find_e_x(e_x, "OLDPWD");
        if (old_path == NULL)
        {
            fprintf(stderr, "OLDPWD not inizialized\n");
            return NULL;
        }
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
            return is_valid_dir(path) == 1 ? arg : NULL;

        return arg; // suite de ../../  blabla
    }
    else
        return is_valid_dir(path) == 1 ? arg : NULL;
}

void change_pwd_var(char *current_path, const char *arg_cd, struct exec_struct *e_x)
{
    if (strcmp(arg_cd, ".") == 0 || strcmp(arg_cd, ";") == 0) // on ne change pas de working dir
    {
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
    }

    else if (strcmp(arg_cd, "..") == 0 || strcmp(arg_cd, "../") == 0) // on recule de 1
    {
        char *s = getenv("PWD");
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
        assign_var("PWD", s, e_x);
    }
    else if (arg_cd[0] == '.' && arg_cd[1] == '.') // str de ../
    {
        char *s = getenv("PWD");
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
        assign_var("PWD", s, e_x);
    }
    else if (strcmp(arg_cd, "-") == 0)
    {
        char *temp = find_e_x(e_x, "OLDPWD");
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
        assign_var("PWD", temp, e_x);
    }
    else if (strcmp(arg_cd, find_e_x(e_x, "OLDPWD")) == 0)
    {
        char *temp = find_e_x(e_x, "OLDPWD");
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
        assign_var("PWD", temp, e_x);
    }
    else if (strcmp(arg_cd, find_e_x(e_x, "HOME")) == 0)
    {
        char *temp = find_e_x(e_x, "PWD");
        assign_var("PWD", (char *) arg_cd, e_x);
        assign_var("OLDPWD", temp, e_x);
    }
    else
    {
        char *temp = find_e_x(e_x, "PWD");
        char *res = final_path((char *)arg_cd, current_path);
        assign_var("OLDPWD", temp, e_x);
        assign_var("PWD", res, e_x);
    }
}

int my_pwd(struct exec_struct *e_x)
{
    printf("%s\n",find_e_x(e_x, "PWD"));
    return 0;
}

int my_cd(char **argv, struct exec_struct *e_x)
{
    int need_pwd = 0;
    char *current_path = find_e_x(e_x, "PWD"); // path avant changement

    const char *arg_cd = get_cd_arg(argv[1], current_path, e_x, &need_pwd); // arg apres cd valid ou pas

    if (arg_cd == NULL)
    {
        fprintf(stderr, "This is not a valid path name argument\n");
        return 2;
    }

    char *temp = final_path((char *) arg_cd, current_path);
    if (chdir(arg_cd) == -1) // change le working dir
    {
        if (chdir(temp) == -1)
        {
            fprintf(stderr, "Chdir failed\n");
            return 2;
        }
    }

    change_pwd_var(current_path, arg_cd, e_x); // met a jour OLDPWD et PWD

    if (need_pwd == 1)
        printf("%s\n", current_path);

    return 0;
}
