//
// Created by clousty8 on 02/12/2021.
//

#include "variable_expention.h"

#include "../memory/hmalloc.h"
#include "string.h"

char *expend(char *start, char *dollar_ind, struct exec_struct *e_x)
{
    char *res = NULL;
    res = hcalloc(1, dollar_ind - start + 8096);
    strcpy(res, strtok(start, "$"));
    for (int i = 0; i < e_x->v_l_size; ++i)
        if (strcmp(e_x->v_l[i].name, dollar_ind) == 0)
            strcat(res, e_x->v_l[i].value);
    return res;
}

char *search_for_dollar(char *word, struct exec_struct *e_x)
{
    for (size_t i = 0; i < strlen(word) - 1; ++i)
        if (word[i] == '$')
            word = expend(word, word + i + 1, e_x);
    return word;
}
