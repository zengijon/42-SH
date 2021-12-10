//
// Created by jennie on 24/11/2021.
//

#include "parser.h"
#inbclude "???" // chemin pour le lexer
#include <err.h>
#include <string.h>

#include "../memory/hmalloc.h"

// weird grammar not chou

struct rule_case *build_case(struct lexer *lex)
{
    struct rule_case *res = hcalloc(1, sizeof(struct rule_case));

    if (lex->current_tok->type != TOKEN_CASE)
        return NULL;

    lexer_pop(lex);
    if (lex->current_tok->type = TOKEN_WORDS)
    {
        res->word = lexer_pop(lex);

        while (lex->current_tok->type = TOKEN_NEWLINE)
            lexer_pop(lex);

        if (lex->current_tok->type != TOKEN_WORDS
            || strcmp("in", lex->current_tok->value) != 0)
            errx(1, "Missing in in case");
        lexer_pop(lex);

        while (lex->current_tok->type = TOKEN_NEWLINE)
            lexer_pop(lex);

        res->case_cl = build_case_clause(lex);

        if (lex->current_tok->type != TOKEN_ESAC)
            errx(2, "missing esac after case");
        lexer_pop(lex);
        return res;
    }
    errx(1, "Missing word after case");
}

static struct case_clause_bis *build_case_clause_bis(struct lexer *lex)
{
    struct case_clause_bis *res = hcalloc(1, sizeof(struct case_clause_bis));
    if (res == NULL)
        errx(1, "calloc failed");
    if (/*lexer |seek |attendu : ;;*/)
    {
        res->is_double_semi = 1;

        while (/*lexer | seek |atten du : \n*/)
            ; // lexer | pop

        if (/*lexer |seek | attendu : case_item*/)
        {
            res->case_it = build_case_item(lex); // lexer |pop

            if (res->case_it == NULL)
                errx(1, "build_case_item failed");
            return res; // pb avec l'etoile est ce au'il faut la repeter
                        // plusieurs fois avec une boucle while ou juste rec
        }
        errx(1, "missing case_item afeter ;;");
    }
    return NULL;
}

struct case_clause *build_case_clause(struct lexer *lex)
{
    struct case_clause *res = hcalloc(1, sizeof(struct cause_clause));

    if ((res->case_it = build_case_item(lex)) == NULL)
        return NULL;

    while (lex->current_tok->type != TOKEN_PTCOMA
           || lexer_peek(lex) != TOKEN_PTCOMA)
    {
        lexer_pop(lex);
        lexer_pop(lex);

        while (lex->current_tok->type = TOKEN_NEWLINE)
            lexer_pop(lex);
        res->next = hrealloc(res->next, ++res->next_size * sizeof(struct case_item));
        if ((res->next[res->next_size - 1] = build_case_item(lex)) == NULL)
            return res;
    }
    while (lex->current_tok->type = TOKEN_NEWLINE)
        lexer_pop(lex);
    return res;
}

static char **word_list_pipe(struct lexer *lex)
{
    char **res = hcalloc(1, sizeof(char *));
    char *temp = hcalloc(2048, sizeof(char));
    int is_once = 0;
    if (res == NULL && temp == NULL)
        errx(1, "calloc failed");

    while (/*lexer |seek | attendu : |*/)
    {
        is_once = 1;
        temp = strcat(temp, "|");
        if (/*lexer |seek | attendu : word*/)
            temp = strcat(temp, " "); // y mettre lexer |pop
        else
            errx(1, "missing word after |");
    }
    return is_once == 1 ? temp : NULL;
}

struct case_item *build_case_item(struct lexer *lex)
{
    struct case_item *res = hcalloc(1, sizeof(struct case_item));

    int is_smth = 0;
    if (lex->current_tok->type == "(")
    {
        is_smth = 1;
        res->is_open_bracket = 1;
    }

    if (lex->current_tok->type == TOKEN_WORDS)
    {
        res->word = lexer_pop(lex);

        while (lex->current_tok->type == "|")
        {
            lexer_pop(lex);
        }

        while (/*lexer |seek | attendu : \n*/)
            ; // lexer | pop

        if (/*lexer | seek | attendu : compound_list*/)
        {
            res->cp_list = build_compound_list(lex); // lexer | pop

            if (res->cp_list == NULL)
                errx(1, "build_compound_list failed");
        }
        return res;
    }
    if (is_smth)
        errx(1, "missing word after (");
    else
        return NULL;
}
