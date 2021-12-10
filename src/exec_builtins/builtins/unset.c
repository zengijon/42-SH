//
// Created by jennie on 09/12/2021.
//

#include "builtins.h"
#include <string.h>
#include <stdlib.h>
#include "../../memory/hmalloc.h"

int remove_var(int i, struct exec_struct *e_x)
{
    assign_var(e_x->v_l[i].name, hcalloc(1, 1), e_x);
    return 0;
}

int remove_fun(int i, struct exec_struct *e_x)
{
    e_x->f_l[i].name = hcalloc(1, 1);
    return 0;
}

int remove_elt(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; ++i)
        if (strcmp(name, e_x->v_l[i].name) == 0)
            remove_var(i, e_x);
    return 0;
}

int my_unset(char **params, struct exec_struct *e_x)
{
    if (params[1] == NULL || params[2] == NULL)
        return 0;
    if (strcmp(params[1], "-v") == 0)
    {
        for (int i = 0; e_x->v_l_size; ++i)
        {
            if (e_x->f_l[i].name == NULL)
                break;
            if (strcmp(params[2], e_x->v_l[i].name))
                remove_var(i, e_x);
        }
    }
    else if (strcmp(params[1], "-f") == 0)
    {
        for (int i = 0; e_x->f_l_len; ++i)
        {
            if (e_x->f_l[i].name == NULL)
                break;
            if (strcmp(params[2], e_x->f_l[i].name) == 0)
                remove_fun(i, e_x);
        }
    }
    else
        remove_elt(e_x, params[1]);
    return 0;
}
