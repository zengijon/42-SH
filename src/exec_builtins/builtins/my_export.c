//
// Created by jennie on 10/12/2021.
//

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
                return 1;
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

int exporting_var(char *params, struct exec_struct *e_x)
{
    char *name = strtok(params, "=");
    char *value;
    if (params == NULL)
    {
        value = find_value_with_name(e_x, params);
        printf("--%s\n", value);
    }
    else
        value = strtok(NULL, "\0");
    setenv(name, value, 1);
    return 0;
}

int my_export(char **params, struct exec_struct *e_x)
{
    if (params[1] == NULL)
        return 0;
    if (strcmp(params[1], "-p") == 0) // need to print the name + value of the exported variable
        return 0;// print_var_env();
    if (is_valid_name(params[1]) == 1)
        return exporting_var(params[1], e_x); // name[=word] => ifndef define it
    return 0;
}

