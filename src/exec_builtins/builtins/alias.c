//
// Created by clousty8 on 12/12/2021.
//

#include "builtins.h"

#include "../../exec/exec.h"
#include "../../parser/parser.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../memory/hmalloc.h"

char *generate_alias(char *input, char *var, char *val)
{
    int max_len = strlen(input) * strlen(var);
    char *buffer = hcalloc(strlen(input) * strlen(var), sizeof(char));
    int len = strlen(var);
    for (int i = 0 ; input[i] != '\0'; ++i)
    {
        if (i == max_len - 1)
        {
            buffer = hrealloc(buffer, max_len * 2);
            max_len *= 2;
        }
        if (strncmp(&input[i], var, len) == 0)
        {
            buffer = strcat(buffer, val);
            i += len - 1;
        }
        else
        {
            char *tmp = calloc(2,1);
            tmp[0] = input[i];
            buffer = strcat(buffer, tmp);
            free(tmp);
        }
    }
    return buffer;
}
