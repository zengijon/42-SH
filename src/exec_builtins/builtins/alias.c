//
// Created by clousty8 on 12/12/2021.
//

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../exec/exec.h"
#include "../../memory/hmalloc.h"
#include "../../parser/parser.h"
#include "builtins.h"

// char *generate_alias(char *input, char *var, char *val)
//{
//     int max_len = strlen(input) * strlen(var);
//     char *buffer = hcalloc(strlen(input) * strlen(var), sizeof(char));
//     int len = strlen(var);
//     for (int i = 0 ; input[i] != '\0'; ++i)
//     {
//         if (i == max_len - 1)
//         {
//             buffer = hrealloc(buffer, max_len * 2);
//             max_len *= 2;
//         }
//         if (strncmp(&input[i], var, len) == 0)
//         {
//             buffer = strcat(buffer, val);
//             i += len - 1;
//         }
//         else
//         {
//             char *tmp = calloc(2,1);
//             tmp[0] = input[i];
//             buffer = strcat(buffer, tmp);
//             free(tmp);
//         }
//     }
//     return buffer;
// }

void add_alias_list(struct exec_struct *ex_l, char *name, char *value)
{
    for (int i = 0; i < ex_l->a_l_size; i++)
        if (strcmp(ex_l->a_l[i].name, name) == 0)
        {
            ex_l->a_l[i].value = value;
            return;
        }
    ex_l->a_l = hrealloc(ex_l->a_l, (++ex_l->a_l_size) * sizeof(char *));
    ex_l->a_l[ex_l->a_l_size - 1].name = name;
    ex_l->a_l[ex_l->a_l_size - 1].value = value;
}

int generate_alias(struct exec_struct *e_x, char **parameters)
{
    for (int i = 0; parameters[i] != 0; ++i)
    {
        if (parameters[i] || fnmatch("*=*", parameters[i], 0) != 0
            || parameters[i][0] == '=')
            return 1;
        char *name = strtok(parameters[i], "=");
        char *value = strtok(NULL, "=");
        add_alias_list(e_x, name, value);
    }
    return 0;
}
