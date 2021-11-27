#include "lexer.h"
#include "utils.h"
#include "../memory/hmalloc.h"
#include "../memory/free_list.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

size_t skipspace(const char *input)
{
    size_t i = 0;
    while (input[i] != 0 && (input[i] == '\t' || input[i] == ' '))
        ++i;
    return i;
}

struct lexer *gestion_quote(struct lexer *lexer, const char *input)
{
    size_t i = 1;
    size_t j = 0;
    char *str = hcalloc(strlen(input), sizeof(char));
    while (input[i] != '\0' && (input[i] == '\\' || input[i] != '\''))
    {
        str[j++] = input[i++];
    }
    lexer->end = lexer->pos + i + 1;
    if (input[i] == '\'')
    {
        lexer->current_tok->type = TOKEN_WORDS;
        lexer->current_tok->value = str;
    }
    else
        lexer->current_tok->type = TOKEN_ERROR;
    return lexer;
}

struct separator *build_separator_list(void)
{
    struct separator *new = hcalloc(1, sizeof(struct separator));
    new->separators = hcalloc(8, sizeof(char *));
    new->separators[0] = hcalloc(2,1);
    new->separators[0] = "\n";
    new->separators[1] = hcalloc(2,1);
    new->separators[1] = " ";
    new->separators[2] = hcalloc(2,1);
    new->separators[2] = ";";
    new->separators[3] = hcalloc(2,1);
    new->separators[3] = "|";
    new->separators[4] = hcalloc(2,1);
    new->separators[4] = "&";
    new->separators[5] = hcalloc(2,1);
    new->separators[5] = "<";
    new->separators[6] = hcalloc(2,1);
    new->separators[6] = ">";
    new->nb_separator = 7;
    return new;
}

int is_separator(const char *input, struct separator *separator)
{
    for (size_t i = 0; i < separator->nb_separator; ++i)
    {
        if(strncmp(input, separator->separators[i], strlen(separator->separators[i])) == 0)
            return 0;
    }
    return 1;
}

int is_token(const char *input, char *token, int n)
{
    char *test = hcalloc(n + 1, 1);
    strcpy(test, token);
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n - 1] = '\n';
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n - 1] = '\0';
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n-1] = '\t';
    if (strncmp(input, test, n) == 0)
        return 0;
    return 1;
}


