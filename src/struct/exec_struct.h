//
// Created by clousty8 on 01/12/2021.
//

#ifndef INC_42SHEEEE_EXEC_STRUCT_H
#define INC_42SHEEEE_EXEC_STRUCT_H

#include <stdio.h>

struct exec_struct
{
    struct var_list *v_l;
    struct redir *r_l;
    int r_l_size;
    int v_l_size;
};

struct redir
{
    int new_fd;
    int old_fd;
    FILE *file;
};

struct var_list
{
    char *name;
    char *value;
    int name_l;
    int value_l;
};

#endif // INC_42SHEEEE_EXEC_STRUCT_H
