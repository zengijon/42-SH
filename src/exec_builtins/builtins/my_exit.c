//
// Created by jennie on 10/12/2021.
//

#include "builtins.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

int is_number(char *word)
{
    for (size_t i = 0; i < strlen(word); ++i)
    {
        if ('0' >= word[i] && word[i] <= '9')
            ;
        else
            return 0;
    }
    return 1;
}

int my_exit(char **parameters)
{
    if (parameters[1] == NULL)
        exit(0);

    else if (is_number(parameters[1]) == 0)
        return 2;

    else
    {
        long long nb = atoi(parameters[1]);

        if (0 <= nb && 255 >= nb)
            exit(nb);

        long long res = nb % 255 - (nb / 255);

        exit(res);
    }
 }

