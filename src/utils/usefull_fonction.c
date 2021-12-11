//
// Created by clousty8 on 08/12/2021.
//

#include "string.h"
#include "../struct/exec_struct.h"

char *get_value_in_vl(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(name, e_x->v_l[i].name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

char *my_itoa(int n, char *s)
{
    int k = 0;
    if (n < 0)
    {
        s[k++] = '-';
        n = -1 * n;
    }
    int i = 1;
    for (int j = n; j >= 10; j /= 10)
        i *= 10;
    for (; i > 0; i /= 10)
    {
        s[k++] = '0' + (n / i);
        n %= i;
    }
    s[k] = 0;
    return s;
}