//
// Created by jennie on 10/12/2021.
//

#include "builtins.h"

#include <stdlib.h>

int my_exit(char **parameters)
{
    if (parameters[1] == NULL)
        exit(0);
    else
    {
        long long nb = atoi(parameters[1]);

        if (0 <= nb && 255 >= nb)
            exit(nb);

        long long res = nb % 255 - (nb / 255);

        printf("%lld\n", res);
        exit(res);
    }
}
