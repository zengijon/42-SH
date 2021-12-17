//
// Created by clousty8 on 02/12/2021.
//

#include "variable_expention.h"

#include <stdio.h>

#include "../memory/hmalloc.h"
#include "../utils/substit.h"
#include "../utils/usefull_fonction.h"
#include "string.h"

int is_accepted(char c, char *accepted)
{
    for (int i = 0; accepted[i] != 0; ++i)
        if (c == accepted[i])
            return 1;
    return 0;
}

int is_escapable(char c)
{
    const char *accepted = "\"\'\n\0";
    for (int i = 0; accepted[i] != 0; ++i)
        if (c == accepted[i])
            return 1;
    return 0;
}

char *get_next(char *word)
{
    int i = 0;
    while (
        is_accepted(
            word[i],
            "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN_0123456789")
            == 1
        || (word[i] == '{'))
        i++;
    return word + i + (word[0] == '{');
}

char *strcmp_withdelim(char *ref, char *start)
{
    int i = 0;
    for (; ref[i] != 0
         && (is_accepted(start[i],
                         "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN_"
                         "0123456789")
                 == 1
             || start[i] == ref[i]);
         i++)
        if (ref[i] != start[i])
            return NULL;
    if (ref[i] != 0
        || is_accepted(start[i],
                       "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN_"
                       "0123456789")
            == 1)
        return NULL;
    return start + i;
}

char *strcmp_withhook(char *ref, char *start)
{
    int i = 0;
    for (; ref[i] != '}' && start[i] != '}' && ref[i] != 0 && start[i] != 0;
         i++)
        if (ref[i] != start[i])
            return NULL;
    if ((ref[i] != 0 && ref[i] != '}') || (start[i] != 0 && start[i] != '}'))
        return NULL;
    return start + i + 1;
}

// corner case when $n is followed by valid charcters. ex : $1a

char *expend(char *start, char *dollar_ind, struct exec_struct *e_x)
{ // unacurate buffer size
    char *res = hcalloc(1, dollar_ind - start + 8096);
    strncpy(res, start, dollar_ind - start - 1);
    if (*(dollar_ind - 1) == '`')
    {
        char *buf = NULL;
        exec_subshell3(dollar_ind, e_x, &buf); // valleur de retour peux échouer
        return strcat(res, buf);
    }
    if (dollar_ind[0] == '(' && dollar_ind[1] != '(')
    {
        char *buf = NULL;
        exec_subshell3(dollar_ind + 1, e_x, &buf); // valleur de retour peux échouer
        return strcat(res, buf);
    }
    char *rest = NULL;
    for (int i = 0; i < e_x->v_l_size; ++i)
    {
        char tmp[8096] = { 0 };
        sprintf(tmp, "{%s}", e_x->v_l[i].name);
        if ((rest = strcmp_withdelim(e_x->v_l[i].name, dollar_ind)) != 0
            || (rest = strcmp_withhook(tmp, dollar_ind)) != 0)
        {
            strcat(res, e_x->v_l[i].value);
            strcat(res, search_for_dollar(rest, e_x));
	    strcat(res, "\0");
            return res;
        }
    }
    strcat(res, search_for_dollar(get_next(dollar_ind), e_x));
    strcat(res, "\0");
    return res;
}

char *search_for_dollar(char *word, struct exec_struct *e_x)
{
    int single = 0;
    int double_ = 0;
    if (word == NULL)
        return hcalloc(1,1);
    for (int i = 0; word[i] != 0 && i < (int) (strlen(word) - 1); i++)
    {
        if (!single && word[i] == '\\')
        {
            i++;
            continue;
        }
        if (single == 0 && word[i] == '"')
            double_ = !double_;
        if (double_ == 0 && word[i] == '\'')
            single = !single;
        if (single == 0 && (word[i] == '$' || word[i] == '`') && word[i + 1] != '"' && word[i + 1] != '^'
            && word[i + 1] != '\'' && word[i + 1] != '\\')
            return expend(word, word + i + 1, e_x);
    }
    return word;
}

char *remove_sep(char *word, struct exec_struct *e_x)
{
    int single = 0;
    int double_ = 0;
    word = search_for_dollar(word, e_x);
    char *res = hcalloc(1, strlen(word) + 1);
    int j = 0;
    for (int i = 0; word[i] != 0; ++i)
    {
        if (single == 0 && word[i] == '"')
            double_ = !double_;
        if (double_ == 0 && word[i] == '\'')
            single = !single;
        if (double_ && is_accepted(word[i], get_value_in_vl(e_x, "IFS")))
        {
            res[j] = '\r';
            j++;
        }
        if ((word[i] == '\"' && single == 0)
            || (word[i] == '\'' && double_ == 0))
            ;
        else if (!single && (!double_ || word[i + 1] == '"' || word[i + 1] == '\\') && word[i] == '\\')
        {
            if ((res[j++] = word[++i]) == 0)
                return res;
        }
        else
        {
            res[j] = word[i];
            j++;
        }
    }
    return res;
}

int valid_name(char *word)
{
    if ((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z')
        || word[0] == '_')
    {
        for (size_t i = 1; i < strlen(word); ++i)
        {
            if (word[i] == '=')
                return 1;
            if (is_accepted(word[i],
                            "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCV"
                            "BN_0123456789")
                == 0)
                return 0;
        }
        return 1;
    }
    return 0;
}
