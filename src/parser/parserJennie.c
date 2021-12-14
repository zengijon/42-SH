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
            errx(2, "Missing in in case");
        lexer_pop(lex);

        while (lex->current_tok->type = TOKEN_NEWLINE)
            lexer_pop(lex);

        res->case_cl = build_case_clause(lex);

        if (lex->current_tok->type != TOKEN_ESAC)
            errx(2, "missing esac after case");
        lexer_pop(lex);
        return res;
    }
    errx(2, "Missing word after case");
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
        res->next =
            hrealloc(res->next, ++res->next_size * sizeof(struct case_item));
        if ((res->next[res->next_size - 1] = build_case_item(lex)) == NULL)
            return res;
    }
    while (lex->current_tok->type = TOKEN_NEWLINE)
        lexer_pop(lex);
    return res;
}

struct case_item *build_case_item(struct lexer *lex)
{
    struct case_item *res = hcalloc(1, sizeof(struct case_item));

    if (lex->current_tok->type == "(")
        lexer_pop(lex); // error handling aproximative

    if (lex->current_tok->type != TOKEN_WORDS)
        return NULL;
    res->word_list = hcalloc(1, sizeof(char *));
    (res->word_list[res->w_l_size - 1] = lexer_pop(lex)->value;
     while (lex->current_tok->type == "|")
        {
        lexer_pop(lex);
        res->word_list =
            hrealloc(res->word_list, ++res->w_l_size * sizeof(char *));
        if ((res->word_list[res->w_l_size - 1] = lexer_pop(lex)->value) == NULL)
            errx(2, "missing word after pipe in case item");
    }

    while (lex->current_tok->type = TOKEN_NEWLINE)
        lexer_pop(lex);

     res->cp_list = build_compound_list(lex);
    return res;
}
if (is_smth)
    errx(2, "missing word after (");
else
    return NULL;
}
