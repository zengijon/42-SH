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
    for (int i = 0; i < e_x->f_l_len; ++i)
        if (strcmp(name, e_x->f_l[i].name) == 0)
            remove_fun(i, e_x);

    for (int i = 0; i < e_x->v_l_size; ++i)
        if (strcmp(name, e_x->v_l[i].name) == 0)
            remove_var(i, e_x);
    return 0;
}

int my_unset(char **params, struct exec_struct *e_x)
{
    if (strcmp(params[0], "-v") == 0)
    {
        for (int i = 0; e_x->v_l_size; ++i)
        {
            if (strcmp(params[1], e_x->v_l[i].name))
                remove_var(i, e_x);
        }
        // il ne faut pas raise un erreur
    }
    else if (strcmp(params[0], "-f") == 0)
    {
        for (int i = 0; e_x->f_l_len; ++i)
            if (strcmp(params[i], e_x->f_l[i].name) == 0)
                remove_fun(i, e_x);
    }
    else
        remove_elt(e_x, params[1]);
    return 0;
}
