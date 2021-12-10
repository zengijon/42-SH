//
// Created by jennie on 10/12/2021.
//

#include "builtins.h"

#include <stdlib.h>

int my_exit(char **parameters)
{
    long long nb = atoi(parameters[1]);

    if (0 <= nb && 255 >= nb)
        exit(nb);
    long long quotient = nb / 255;
    long long res = nb % 255 - quotient;
    printf("%lld\n",res);
    exit(res);
}
