#define _GNU_SOURCE //should be removed for mason

#include "lexer.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <fnmatch.h>

#include "../memory/free_list.h"
#include "../memory/hmalloc.h"
#include "utils.h"

//struct free_list *list_malloc = NULL;

struct lexer *lexer_new(const char *input)
{
    struct lexer *res = hcalloc(1, sizeof(struct lexer));
    res->input = input;
    res->pos = skipspace(input);
    res->current_tok = hcalloc(1, sizeof(struct token));
    struct separator *separator = build_separator_list();
    if (input[res->pos] == '\0')
        res->current_tok->type = TOKEN_EOF;
    if (is_token(&input[res->pos], "if", 2) == 0
	&& (is_separator(input + res->pos + 2, separator) == 0))
    {
        res->current_tok->type = TOKEN_IF;
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "then", 4) == 0
	&& (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_THEN;
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "elif",4) == 0
	     && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ELIF;
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "else",4) == 0
	     && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ELSE;
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "fi", 2) == 0
             && is_separator(input + res->pos + 2, separator) == 0)
    {
        res->current_tok->type = TOKEN_FI;
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "while", 5) == 0
	     && (is_separator(input + res->pos + 5, separator) == 0))
    {
        res->current_tok->type = TOKEN_WHILE;
        res->end = res->pos + 5;
    }
    else if (is_token(&input[res->pos], "for", 3) == 0
	     && (is_separator(input + res->pos + 3, separator) == 0))
    {
        res->current_tok->type = TOKEN_FOR;
        res->end = res->pos + 3;
    }
    else if (is_token(&input[res->pos], "do", 2) == 0
	     && (is_separator(input + res->pos + 2, separator) == 0))
    {
        res->current_tok->type = TOKEN_DO;
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "done",4) == 0
	     && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_DONE;
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "case", 2) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_CASE;
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "esac",4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ESAC;
        res->end = res->pos + 4;
    }
    else if (strncmp(&input[res->pos], ";", 1) == 0)
    {
        res->current_tok->type = TOKEN_PTCOMA;
        res->end = res->pos + 1;
    }
    else if (strncmp(&input[res->pos], "\n", 1) == 0)
    {
        res->current_tok->type = TOKEN_NEWLINE;
        res->end = res->pos + 1;
    }
    else if (strncmp(&input[res->pos], "\"", 1) == 0)
        res = gestion_double_quote(res, &input[res->pos]);
    else if (strncmp(&input[res->pos], "'", 1) == 0)
        res = gestion_quote(res, &input[res->pos]);
    else if (strncmp(&input[res->pos], "&", 1) == 0
             || strncmp(&input[res->pos], "|", 1) == 0)
    {
        res = gestion_and_or(res, &input[res->pos]);
    }
    else if (input[res->pos] == '(')
    {
        res->current_tok->type = TOKEN_PA_OPEN;
        res->end = res->pos + 1;
    }
    else if (input[res->pos] == ')')
    {
        res->current_tok->type = TOKEN_PA_CLOSE;
        res->end = res->pos + 1;
    }
    else if (input[res->pos] == '{')
    {
        res->current_tok->type = TOKEN_ACO_OPEN;
        res->end = res->pos + 1;
    }
    else if (input[res->pos] == '}')
    {
        res->current_tok->type = TOKEN_ACO_CLOSE;
        res->end = res->pos + 1;
    }
    else if (fnmatch("*([0-9])[<>]?([<>|&])*", input + res->pos, FNM_EXTMATCH)
             == 0)
    {
        gestion_redir(res, input + res->pos);
    }
    else
    {
        size_t j = 0;
        size_t k = res->pos;
        char *value = hcalloc(strlen(input) + 1, sizeof(char));
        while (input[k] != '\0' && is_separator(&input[k], separator) != 0)
        {
            if (input[k] == '\\')
            {
                value[j++] = input[k++];
                value[j++] = input[k++];
            }
            else
                value[j++] = input[k++];
        }
        res->current_tok->type = TOKEN_WORDS;
        res->current_tok->value = value;
        res->end = k;
    }
    return res;
}

void lexer_free(struct lexer *lexer)
{
    token_free(lexer->current_tok);
}

struct token *lexer_peek(struct lexer *lexer)
{
    struct separator *separator = build_separator_list();
    if (lexer->input[lexer->pos] == '\'')
    {
        separator->nb_separator = 1;
        separator->separators[0] = "\'";
    }
    int i = lexer->end;
    while (lexer->input[i] != '\0' && lexer->input[i] == ' ')
        i++;
    if (lexer->input[i] == '\0')
    {
        return token_new(TOKEN_EOF);
    }
    i += skipspace(&lexer->input[i]);
    struct lexer *tmp = lexer_new(lexer->input + i);
    return tmp->current_tok;
}
struct token *lexer_peek_rec(struct lexer *lexer, int n)
{
    struct separator *separator = build_separator_list();
    if (lexer->input[lexer->pos] == '\'')
    {
        separator->nb_separator = 1;
        separator->separators[0] = "\'";
    }
    int i = lexer->end;
    while (lexer->input[i] != '\0' && lexer->input[i] == ' ')
        i++;
    if (lexer->input[i] == '\0')
    {
        return token_new(TOKEN_EOF);
    }
    i += skipspace(&lexer->input[i]);
    struct lexer *tmp = lexer_new(lexer->input + i);
    if (n == 1)
        return tmp->current_tok;
    else
        return lexer_peek_rec(tmp, n - 1);
}

struct token *lexer_pop(struct lexer *res)
{
    struct separator *separator = build_separator_list();
    if (res->input[res->pos] == '\'')
    {
        separator->nb_separator = 1;
        separator->separators[0] = "'";
    }
    struct token *tmp = hcalloc(1, sizeof(struct token));
    tmp->type = res->current_tok->type;
    tmp->value = res->current_tok->value;
    size_t i = res->end;
    while (res->input[i] != '\0' && res->input[i] == ' ')
        i++;
    if (res->input[i] == '\0')
    {
        res->current_tok->type = TOKEN_EOF;
        res->pos = i - 1;
        return tmp;
    }
    const char *input = res->input;
    i += skipspace(&res->input[i]);
    res->pos = i;
    if (is_token(&input[res->pos], "if", 2) == 0
        && (is_separator(input + res->pos + 2, separator) == 0))
    {
        res->current_tok->type = TOKEN_IF;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "if";
        }
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "then", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_THEN;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "then";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "elif", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ELIF;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "elif";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "else", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ELSE;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "else";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "case", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_CASE;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "case";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "esac", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_ESAC;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "esac";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "fi", 2) == 0
             && (is_separator(input + res->pos + 2, separator) == 0))
    {
        res->current_tok->type = TOKEN_FI;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "fi";
        }
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "while", 5) == 0
             && (is_separator(input + res->pos + 5, separator) == 0))
    {
        res->current_tok->type = TOKEN_WHILE;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "while";
        }
        res->end = res->pos + 5;
    }
    else if (is_token(&input[res->pos], "for", 3) == 0
             && (is_separator(input + res->pos + 3, separator) == 0))
    {
        res->current_tok->type = TOKEN_FOR;
        if (tmp->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "for";
        }
        res->end = res->pos + 3;
    }
    else if (is_token(&input[res->pos], "do", 2) == 0
             && (is_separator(input + res->pos + 2, separator) == 0))
    {
        res->current_tok->type = TOKEN_DO;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "do";
        }
        res->end = res->pos + 2;
    }
    else if (is_token(&input[res->pos], "done", 4) == 0
             && (is_separator(input + res->pos + 4, separator) == 0))
    {
        res->current_tok->type = TOKEN_DONE;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "done";
        }
        res->end = res->pos + 4;
    }
    else if (is_token(&input[res->pos], "{", 1) == 0
             && (is_separator(input + res->pos + 1, separator) == 0))
    {
        res->current_tok->type = TOKEN_ACO_OPEN;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "{";
        }
        res->end = res->pos + 1;
    }
    else if (is_token(&input[res->pos], "}", 1) == 0
             && (is_separator(input + res->pos + 1, separator) == 0))
    {
        res->current_tok->type = TOKEN_ACO_CLOSE;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "}";
        }
        res->end = res->pos + 1;
    }
    else if (is_token(&input[res->pos], "until", 5) == 0
             && (is_separator(input + res->pos + 5, separator) == 0))
    {
        res->current_tok->type = TOKEN_UNTIL;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "until";
        }
        res->end = res->pos + 5;
    }
    else if (is_token(&input[res->pos], "!", 1) == 0
             && (is_separator(input + res->pos + 1, separator) == 0))
    {
        res->current_tok->type = TOKEN_NEG;
        if (res->current_tok->type == TOKEN_WORDS)
        {
            res->current_tok->type = TOKEN_WORDS;
            res->current_tok->value = "!";
        }
        res->end = res->pos + 1;
    }
    else if (strncmp(&input[res->pos], ";", 1) == 0)
    {
        res->current_tok->type = TOKEN_PTCOMA;
        res->end = res->pos + 1;
    }
    else if (strncmp(&input[res->pos], "\n", 1) == 0)
    {
        res->current_tok->type = TOKEN_NEWLINE;
        res->end = res->pos + 1;
    }
    else if (input[res->pos] == '(')
    {
        res->current_tok->type = TOKEN_PA_OPEN;
        res->end = res->pos + 1;
    }
    else if (input[res->pos] == ')')
    {
        res->current_tok->type = TOKEN_PA_CLOSE;
        res->end = res->pos + 1;
    }
    else if (strncmp(&input[res->pos], "\"", 1) == 0)
        res = gestion_double_quote(res, &input[res->pos]);
    else if (strncmp(&input[res->pos], "'", 1) == 0)
        res = gestion_quote(res, &input[res->pos]);
    else if (strncmp(&input[res->pos], "&", 1) == 0
             || strncmp(&input[res->pos], "|", 1) == 0)
        res = gestion_and_or(res, &input[res->pos]);
    else if (fnmatch("*([0-9])[<>]?([<>|&])*", input + res->pos, FNM_EXTMATCH)
             == 0)
        gestion_redir(res, input + res->pos);
    else
    {
        res->current_tok->type = TOKEN_WORDS;
        size_t j = 0;
        size_t k = res->pos;
        char *value = hcalloc(strlen(input) + 1, sizeof(char));
        while (input[k] != '\0' && is_separator(&input[k], separator) != 0)
        {
            if (input[k] == '$' && input[k + 1] == '(')
            {
                int is_sub = 1;
                value[j++] = input[k++];
                value[j++] = input[k++];
                while (input[k] != '\0' && is_sub > 0)
                {
                    if (input[k] == '(')
                        is_sub++;
                    if (input[k] == ')')
                        is_sub--;
                    value[j++] = input[k++];
                }
                if (input[k] == '\0')
                    res->current_tok->type = TOKEN_ERROR;
            }
            if (input[k] == '\\')
            {
                value[j++] = input[k++];
                value[j++] = input[k++];
            }
            else
                value[j++] = input[k++];
        }
        res->current_tok->value = value;
        res->end = k;
    }
    return tmp;
    //     tok = lexer_pop(lexer);
    //     printf("%d\n", tok->type); //9
    //     tok = lexer_pop(lexer);
}

//int main(void)
//{
//    struct lexer *lexer = lexer_new("3kfdffff\necho a\n echo b");
//    //    printf("%d\n", lexer->current_tok->type);
//    //    struct token *tok = lexer_peek(lexer);
//    //    printf("%d\n", tok->type);
//    struct token *tok = lexer_pop(lexer);
//    while (tok->type != TOKEN_EOF)
//        tok = lexer_pop(lexer);
//
//
//    //    printf("==================================\n");
//    //    tok = lexer_pop(lexer);
//    //    printf("%s\n", tok->value);
//    //    tok = lexer_peek(lexer);
//    //    printf("%s\n", tok->value);
//    //    printf("====================================\n");
//    //    printf("%s\n", lexer->current_tok->value);
//    return 0;
//}
