//
// Created by jennie on 06/12/2021.
//

#include <ctype.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../memory/hmalloc.h"
#include "builtins.h"

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

    closedir(pDir);
    return 1;
}

char *find_e_x(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(e_x->v_l[i].name, name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

char *get_cd_arg(char *arg, char *current_path, struct exec_struct *e_x,
                       int *need_pwd)

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

int count_par(const char *arg_cd)
{
    int count = 0;
    for (int i = 0; arg_cd[i] != '\0'; ++i)
        if (arg_cd[i] == '/')
            count++;
    return count;
}

char *for_double_pts(int len, char *current_path, char *res) // res deja calloc
{
    int i = len - 1;
    while (i >= 0)
    {
        if (current_path[i] == '/')
        {
            current_path[i] = '&';
            break;
        }
        i--;
    }
    memccpy(res, current_path, '&', len - i - 1);
    res[len - i - 2] = '/';
    return res;
}

char *change_path(char *current_path, const char *arg_cd)
{
    char *res = hcalloc(1, strlen(current_path) + strlen(arg_cd) + 25);
    int len = (int)strlen(current_path);
    if (strcmp(current_path, "/") == 0)
        return current_path;
    if (strcmp(arg_cd, "..") == 0 || strcmp(arg_cd, "../") == 0)
        res = for_double_pts(len, current_path, res);

    else
    {
        ;
        // si on a beaucoup de ../ ../
        // si on a ../dir ou ../../ ... /dir
    }
    return res;
}

void change_pwd_var(char *current_path, const char *arg_cd,
                    struct exec_struct *e_x)
{
    if (strcmp(arg_cd, ".") == 0
        || strcmp(arg_cd, ";") == 0) // on ne change pas de working dir
    {
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
    }

    else if (strcmp(arg_cd, "..") == 0
             || strcmp(arg_cd, "../") == 0) // on recule de 1
    {
        char *s = change_path(current_path, arg_cd);
        assign_var("OLDPWD", find_e_x(e_x, "PWD"), e_x);
        assign_var("PWD", s, e_x);
    }
    else if (arg_cd[0] == '.' && arg_cd[1] == '.') // str de ../
    {
        char *s = change_path(current_path, arg_cd);
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
        assign_var("PWD", (char *)arg_cd, e_x);
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
    printf("%s\n", find_e_x(e_x, "PWD"));
    return 0;
}

void change_pwd_var2(char *current_path, const char *arg_cd, struct exec_struct *e_x);


int my_cd(char **argv, struct exec_struct *e_x)
{
    int need_pwd = 0;
    char *current_path = find_e_x(e_x, "PWD"); // path avant changement
    if (current_path == NULL)
    {
        fprintf(stderr, "PWD has not yet been define\n");
    }
    char *arg_cd = get_cd_arg(argv[1], current_path, e_x,
                                    &need_pwd); // arg apres cd valid ou pas

    if (arg_cd == NULL)
    {
        fprintf(stderr, "This is not a valid path name argument\n");
        return 2;
    }

    //gerer les chemins ABS
    if (*arg_cd == '/')
    {
        assign_var("OLDPWD", current_path, e_x);
        assign_var("PWD", arg_cd, e_x);
    }
    else
        change_pwd_var2(current_path, arg_cd, e_x);
    if (chdir(find_e_x(e_x, "PWD")) == -1) // change le working dir
    {
            fprintf(stderr, "Chdir failed\n");
            return 2;
    }

     // met a jour OLDPWD et PWD

    if (need_pwd == 1)
        printf("%s\n", current_path);

    return 0;
}

int poplast(char *newpath)
{
    if (*newpath == 0)
        return 1;
    int res = poplast(1 + newpath);
    if (res && *newpath == '/')
    {
        *newpath = 0;
        return 0;
    }
    return res;
}

void change_pwd_var2(char *current_path, const char *arg_cd, struct exec_struct *e_x)
{
    char *newpath = hcalloc(10, strlen(current_path));
    strcpy(newpath, current_path);
    char *trash = hcalloc(1, strlen(arg_cd));
    strcpy(trash, arg_cd);
    char *tmp = NULL;
    (tmp = strtok(trash, "/"));
    do {
        if (strcmp(tmp, ".") == 0)
            ;
        else if (strcmp(tmp, "..") == 0)
        {
                poplast(newpath);
        }
        else
        {
            strcat(newpath, "/");
            strcat(newpath, tmp);
        }
    } while ((tmp = strtok(NULL, "/")) != 0);
    if (strlen(newpath) == 0)
        strcat(newpath, "/");
    assign_var("OLDPWD", current_path, e_x);
    assign_var("PWD", newpath, e_x);
}
