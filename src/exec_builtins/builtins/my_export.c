//
// Created by jennie on 10/12/2021.
//
#include <err.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

int is_valid_name(char *name)
{
    size_t len = strlen(name);
    if (('a' <= name[0] && name[0] <= 'z') || ('A' <= name[0] && name[0] <= 'Z'))
    {
        for (size_t i = 1; i < len; ++i)
        {
            if ('a' <= name[i] && name[i] <= 'z')
                ;
            else if ('A' <= name[i] && name[i] <= 'Z')
                ;
            else if ('0' <= name[i] && name[i] <= '9')
                ;
            else if (name[i] == '_')
                ;
            else if (name[i] == '=')
                break;
            else
                return 0;
        }
        return 1;
    }
    return 0;
}
char *find_value_with_name(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(e_x->v_l[i].name, name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}
int is_equal(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
        if (str[i] == '=')
            return 1;
    return 0;
}
int exporting_var(char *params, struct exec_struct *e_x)
{
    char *name;
    char *value;
    if (is_equal(params) == 1)
    {
        name = strtok(params, "=");
        value = strtok(NULL, "\0");
    }
    else
    {
        name = params;
        value = find_value_with_name(e_x, params);
    }

    setenv(name, value, 1);

    return 0;
}

int my_export(char **params, struct exec_struct *e_x)
{
    if (params[1] == NULL)
        return 0;
    if (strcmp(params[1], "-p") == 0) // need to print the name + value of the exported variable
        return 0;
    if (is_valid_name(params[1]) == 1)
        return exporting_var(params[1], e_x);
    else
        errx(2, "Bad name for export");
}

