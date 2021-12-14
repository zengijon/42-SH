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

//size_t len_path(const char *current_path)
//{
//    size_t nb = 0;
//    if (current_path != NULL)
//    {
//        for (int i = 0; current_path[i] != '\0'; ++i)
//        {
//            if (current_path[i] == '/')
//                nb+= 1;
//        }
//    }
//    return nb;
//}

int is_valid_dir(char *path)
{
    DIR *pDir = opendir(path);
    if (pDir == NULL)
    {
        //printf ("Cannot open directory\n");
        return 0;
    }
    closedir (pDir);
    return 1;
}

//int is_valid_d_coma(char *arg, size_t nb_tok_path);
int is_valid_d_coma(char *arg)
{
    size_t len = strlen(arg);
    size_t count = 0;
    size_t i = 0;
    if (len % 3 == 0)
    {
        while (i < len)
        {
            //if (count >= nb_tok_path)
                //errx(2, "sequence of ../ is too long");

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

char *find_e_x(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(e_x->v_l[i].name, name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

//const char *get_cd_arg(char *arg, char *current_path, size_t nb_tok_path, struct exec_struct *e_x, int *need_pwd);
const char *get_cd_arg(char *arg, char *current_path, struct exec_struct *e_x, int *need_pwd)

{
    if (arg == NULL)
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

        //return is_valid_d_coma(arg, nb_tok_path) == 1 ? arg : NULL;
        return is_valid_d_coma(arg) == 1 ? arg : NULL;
    }
    else
        return is_valid_dir(path) == 1 ? arg : NULL;
}

void change_pwd_var(char *current_path, const char *arg_cd, struct exec_struct *e_x)
{
    if (strcmp(arg_cd, ".") == 0)
        assign_var("OLDPWD", current_path, e_x);

    else if (strcmp(arg_cd, "..") == 0 || strcmp(arg_cd, "../") == 0)
    {
        char s[2048] = { 0 };
        getcwd(s, 2048);
        assign_var("OLDPWD", s, e_x);
    }
    else if (arg_cd[0] == '.' && arg_cd[1] == '.')
    {
        char s[2048] = { 0 };
        getcwd(s, 2048);
        assign_var("OLDPWD", s, e_x);
    }
    else if (strcmp(arg_cd, "-") == 0)
        ;
    else if (strcmp(arg_cd, "/home") == 0)
        ;
    else if (strcmp(arg_cd, find_e_x(e_x, "HOME")) == 0)
        ;
    else
    {
        char * temp = final_path((char *)arg_cd, current_path);
        assign_var("OLDPWD", temp, e_x);
    }
}

int my_pwd(struct exec_struct *e_x)
{
    printf("%s\n",find_e_x(e_x, "OLDPWD"));
    return 0;
}

int my_cd(char **argv, struct exec_struct *e_x)
{
    int need_pwd = 0;
    char *current_path = find_e_x(e_x, "OLDPWD");

    //size_t len_current_path = len_path(current_path);

    //const char *arg_cd = get_cd_arg(argv[1], current_path, len_current_path, e_x, &need_pwd);
    const char *arg_cd = get_cd_arg(argv[1], current_path, e_x, &need_pwd);

    if (arg_cd == NULL)
    {
        fprintf(stderr, "This is not a valid path name argument\n");
        return 2;
    }

    if (chdir(arg_cd) == -1)
    {
        fprintf(stderr, "Chdir failed\n");
        return 2;
    }

    change_pwd_var(current_path, arg_cd, e_x);

//    char s[2048] = { 0 };
//    getcwd(s, 2048);
//    printf("---%s\n", s);

    if (need_pwd == 1)
        printf("%s\n", current_path);

    return 0;
}
