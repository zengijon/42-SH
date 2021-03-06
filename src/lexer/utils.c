#include "utils.h"

#include <err.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/free_list.h"
#include "../memory/hmalloc.h"
#include "lexer.h"
#include "utils.h"

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
    size_t j = 1;
    char *str = hcalloc(strlen(input), sizeof(char) + 1);
    strcat(str, "'");
    while (input[i] != '\0' && input[i] != '\'')
    {
        str[j++] = input[i++];
    }
    strcat(str, "'");
    lexer->end = lexer->pos + i + 1;
    if (input[i] == '\'')
    {
        lexer->current_tok->type = TOKEN_WORDS;
        lexer->current_tok->value = str;
    }
    else
        errx(2, "missing single quote");
    return lexer;
}

struct lexer *gestion_double_quote(struct lexer *lexer, const char *input)
{
    size_t i = 1;
    size_t j = 1;
    char *str = hcalloc(strlen(input), sizeof(char) + 1);
    strcat(str, "\"");
    while (input[i] != '\0' && input[i] != '\"')
    {
        if (input[i] == '\\')
            i++;
        str[j++] = input[i++];
    }
    strcat(str, "\"");
    lexer->end = lexer->pos + i + 1;
    if (input[i] == '\"')
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
    new->separators = hcalloc(20, sizeof(char *));
    new->separators[0] = hcalloc(2, 1);
    new->separators[0] = "\n";
    new->separators[1] = hcalloc(2, 1);
    new->separators[1] = " ";
    new->separators[2] = hcalloc(2, 1);
    new->separators[2] = ";";
    new->separators[3] = hcalloc(2, 1);
    new->separators[3] = "|";
    new->separators[4] = hcalloc(2, 1);
    new->separators[4] = "&";
    new->separators[5] = hcalloc(2, 1);
    new->separators[5] = "<";
    new->separators[6] = hcalloc(2, 1);
    new->separators[6] = ">";
    new->separators[7] = hcalloc(2, 1);
    new->separators[7] = "(";
    new->separators[8] = hcalloc(2, 1);
    new->separators[8] = ")";
    new->nb_separator = 9;
    return new;
}

int is_separator(const char *input, struct separator *separator)
{
    for (size_t i = 0; i < separator->nb_separator; ++i)
    {
        if (strncmp(input, separator->separators[i], strlen(separator->separators[i]))== 0)
            return 0;
    }
    if (input[0] == '\0')
        return 0;
    return 1;
}

int is_token(const char *input, char *token, int n)
{
    char *test = hcalloc(n + 1, 10);
    strcpy(test, token);
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n] = '\n';
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n] = '\0';
    if (strncmp(input, test, n) == 0)
        return 0;
    test[n] = '\t';
    if (strncmp(input, test, n) == 0)
        return 0;
    return 1;
}

struct lexer *gestion_and_or(struct lexer *lexer, const char *input)
{
    if (input[0] == input[1])
    {
        lexer->end = lexer->pos + 2;
        if (*input == '&')
            lexer->current_tok->type = TOKEN_AND;
        else
            lexer->current_tok->type = TOKEN_OR;
    }
    else
    {
        lexer->end = lexer->pos + 1;
        if (*input == '&')
            lexer->current_tok->type = TOKEN_ESP;
        else
            lexer->current_tok->type = TOKEN_PIPE;
    }
    return lexer;
}

struct lexer *gestion_redir(struct lexer *lexer, const char *input)
{
    int i = 0;
    lexer->current_tok->type = TOKEN_REDIR;
    char *res = hcalloc(1500, sizeof(char));
    while (input[i] != '>' && input[i] != '<')
    {
        res[i] = input[i];
        ++i;
    }
    res[i] = input[i];
    if ((input[i + 1] == '>' || input[i + 1] == '<')
        && input[i] != input[i + 1])
    {
        lexer->end = lexer->pos + i + 1;
    }
    else if (fnmatch("[<>|&]*", input + i + 1, 0) == 0)
    {
        lexer->end = lexer->pos + i + 2;
        res[i + 1] = input[i + 1];
    }
    else
    {
        lexer->end = lexer->pos + i + 1;
    }
    lexer->current_tok->value = res;
    return lexer;
}

struct lexer *build_command_sub(struct lexer *lexer, const char *input)
{
    int i = 2;
    int nb_parenthese = 1;
    lexer->current_tok->type = TOKEN_WORDS;
    char *buffer = hcalloc(strlen(input) + 1, sizeof(char));
    buffer[0] = '$';
    buffer[1] = '(';
    while (input[i] != '\0' && nb_parenthese > 0)
    {
        if (input[i] == '(')
            nb_parenthese++;
        if (input[i] == ')')
            nb_parenthese--;
        buffer[i] = input[i];
        ++i;
    }
    if (input[i] == '\0')
        lexer->current_tok->type = TOKEN_ERROR;
    lexer->end = lexer->pos + i;
    return lexer;
}

const char *build_input(struct lexer *lexer, char *value, struct token *token)
{
    int i = 0;
    char *buffer = hcalloc(strlen(value) + strlen(lexer->input) + 1, sizeof(char));
    while (i != (int) lexer->end)
    {
        buffer[i] = lexer->input[i];
        ++i;
    }
    buffer = strcat(buffer, value);
    buffer = strcat(buffer, &lexer->input[lexer->end + strlen(token->value)]);
    return buffer;
}

struct lexer *generate_alias_(struct lexer *lexer, struct token *token)
{
    for (int i = 0; i < lexer->e_x->a_l_size; ++i)
    {
        if (strcmp(token->value, lexer->e_x->a_l[i].name) == 0)
        {
            const char *new_input = build_input(lexer, lexer->e_x->a_l[i].value, token);
            lexer->input = new_input;
        }
    }
    return lexer;
}
