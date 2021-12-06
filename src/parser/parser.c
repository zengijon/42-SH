//
// Created by clousty8 on 24/11/2021.
//

#include "parser.h"

#include <err.h>
#include <fnmatch.h>
#include <string.h>

#include "../memory/hmalloc.h"
#include "../exec/variable_expention.h"

struct list_next *build_list_next(struct lexer *lex)
{
    struct list_next *res = hcalloc(1, sizeof(struct list_next));
    res->a_o = build_and_or(lex);
    if (res->a_o == NULL)
        return NULL;
    if (lex->current_tok->type == TOKEN_PTCOMA)
        ;
    else if (lex->current_tok->type == TOKEN_ESP)
        res->esp = 1;
    else if (lex->current_tok->type == TOKEN_NEWLINE)
        return res;
    else
        errx(1, "missing separator after list");
    lexer_pop(lex);

    res->next = build_list_next(lex);

    return res;
}

struct list *build_list(struct lexer *lex)
{
    struct list *res = hcalloc(1, sizeof(struct list));
    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);
    res->a_o = build_and_or(lex);
    if (res->a_o == NULL)
        return NULL;
    if (lex->current_tok->type == TOKEN_PTCOMA)
        ;
    else if (lex->current_tok->type == TOKEN_ESP)
        res->esp = 1;
    else if (lex->current_tok->type == TOKEN_NEWLINE || lex->current_tok->type == TOKEN_EOF)
        return res;
    else
        errx(1, "missing separator after list");
    lexer_pop(lex);
    res->next = build_list_next(lex);
    return res;
}

struct and_or_next *build_and_or_next(struct lexer *lex)
{
    struct and_or_next *res = hcalloc(1, sizeof(struct and_or_next));

    if (lex->current_tok->type != TOKEN_AND
        && lex->current_tok->type != TOKEN_OR)
        return NULL;
    res->op = lex->current_tok->type == TOKEN_AND ? 1 : 2;
    lexer_pop(lex);
    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);

    if ((res->pipeline = build_pipeline(lex)) == NULL)
        errx(1, "Missing pipeline after operator");

    res->next = build_and_or_next(lex);
    return res;
}

struct and_or *build_and_or(struct lexer *lex)
{
    struct and_or *res = hcalloc(1, sizeof(struct and_or));
    res->pipeline = build_pipeline(lex);
    if (res->pipeline == NULL)
        return NULL;

    res->next = build_and_or_next(lex);
    return res;
}

struct pipeline_next *build_pipeline_next(struct lexer *lex)
{
    struct pipeline_next *res = hcalloc(1, sizeof(struct pipeline_next));
    if (lex->current_tok->type == TOKEN_PIPE)
    {
        lexer_pop(lex);

        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);

        res->cmd = build_command(lex);
        if (res->cmd != NULL)
        {
            res->next = build_pipeline_next(lex);
            return res;
        }
        errx(1, "Missing cmd after | and new line");
    }
    return NULL;
}

struct pipeline *build_pipeline(struct lexer *lex)
{
    struct pipeline *res = hcalloc(1, sizeof(struct pipeline));
    if (lex->current_tok->type == TOKEN_NEG)
    {
        res->negation = 1;
        lexer_pop(lex);
    }
    if ((res->cmd = build_command(lex)) == NULL)
    {
        if (res->negation == 0)
            return NULL;
        else
            errx(1, "missing command after negation mark '!' ");
    }
    res->next = build_pipeline_next(lex);

    return res;
}

struct command *build_command(struct lexer *lex)
{
    struct command *res = hcalloc(1, sizeof(struct command));
    if ((res->s_cmd = build_simple_command(lex)) != NULL)
        return res;
    else if ((res->sh_cmd = build_shell_command(lex)) != NULL)
        ;
    //    else if ((res->fun = build_funcdec(lex)) != NULL)
    //        ;
    else
        return NULL;
    //    res->redir = build_redirection(lex);
    return res;
}

struct simple_command *build_simple_command(struct lexer *lex)
{
    struct simple_command *res = hcalloc(1, sizeof(struct simple_command));
    struct prefix *tmp = NULL;
    while ((tmp = build_prefix(lex)) != NULL)
    {
        res->list_pre = hrealloc(res->list_pre,
                                 ++(res->size_pre) * sizeof(struct prefix *));
        res->list_pre[res->size_pre - 1] = tmp;
    }

    struct element *tmp2 = NULL;
    while ((tmp2 = build_element(lex)) != NULL)
    {
        res->list_elt = hrealloc(res->list_elt,
                                 ++(res->size_elt) * sizeof(struct element *));
        res->list_elt[res->size_elt - 1] = tmp2;
    }

    if (res->list_pre == NULL && res->list_elt == NULL)
        return NULL;

    return res;
}

struct shell_command *build_shell_command(struct lexer *lex)
{
    struct shell_command *res = hcalloc(1, sizeof(struct shell_command));

    //    if (lex->current_tok->type == "{")
    //    {
    //        lexer_pop(lex);
    //        res->c_p = build_compound_list(lex);
    //        if (res->c_p == NULL)
    //            errx(1, "missing compound list between '{}'");
    //        if (lex->current_tok-type != "}")
    //            errx(1, "syntax error : missing '}'");
    //        lexer_pop(lex);
    //        return res;
    //    }
    //
    //    if (lex->current_tok->type == "(")
    //    {
    //        lexer_pop(lex);
    //        res->c_p = build_compound_list(lex);
    //        if (res->c_p == NULL)
    //            errx(1, "missing compound list between '()'");
    //        if (lex->current_tok->type != ")")
    //            errx(1, "syntax error : missing ')'");
    //        lexer_pop(lex);
    //        return res;
    //    }
    //
    if ((res->r_f = build_rule_for(lex)) != NULL)
        ;
    else if ((res->r_w = build_rule_while(lex)) != NULL)
        ;
    else if ((res->r_u = build_rule_until(lex)) != NULL)
        ;
    //    else if ((res->r_c = build_rule_case(lex)) != NULL)
    //        ;
    else if ((res->r_i = build_rule_if(lex)) != NULL)
        ;
    else
        return NULL;
    return res;
}

// struct funcdec *build_funcdec(struct lexer *lex)
//{
//     struct funcdec *res = hcalloc(1, sizeof(struct funcdec));
//
//     if (lex->current_tok->type == TOKEN_WORDS)
//     {
//         if (lexer_peek(lex)->type != "(")
//              return NULL;
//         if (lexer_peek_rec(lex,2)->type != ')')
//             return NULL;
//         res->funct_name = lexer_pop(lex)->value;
//         lexer_pop(lex);
//         lexer_pop(lex);
//
//         while (lex->current_tok->type == TOKEN_NEWLINE)
//             lexer_pop(lex);
//         res->sh_cmd = build_shell_command(lex);
//
//         if (res->sh_cmd == NULL)
//             errx(1, "Missing shell cmd after word () new line");
//         return res;
//     }
//     return NULL;
// }

// struct redirection *build_redirection(struct lexer *lex)
//{
//     struct redirection *res = hcalloc(1, sizeof(struct funcdec));
//     if (lex->current_tok->type == io_nb)
//         res->IONUMBER = lexer_pop(lex) - '0';
//     else
//         res->IONUMBER = -1;
//     if (lex->current_tok->type != )
//         return NULL;
//     res->re_op = 0; // lexer | pop
//     if ( lexer | pop | reçu != word)
//         errx (1, "missing file name to redirect to");
//     res->next = build_redirection();
//     return res;
// }

struct prefix *build_prefix(struct lexer *lex)
{
    struct prefix *res = hcalloc(1, sizeof(struct prefix));

    if (lex->current_tok->type != TOKEN_WORDS
        || fnmatch("*=*", lex->current_tok->value, 0) != 0
        || lex->current_tok->value[0] == '=')
        return NULL;
    res->assignment_word = lexer_pop(lex)->value;
    if (valid_name(res->assignment_word) == 0)
        err(1, "name not valid");
    // res->redirect = build_redirection(lex);

    return res;
}

struct element *build_element(struct lexer *lex)
{
    struct element *res = hcalloc(1, sizeof(struct element));

    if (lex->current_tok->type != TOKEN_WORDS)
        return NULL;
    res->word = lexer_pop(lex)->value;

    //    res->redirect = build_redirection(lex);

    return res;
}

#include "stdio.h"

static struct compound_next *build_compound_next(struct lexer *lex)
{
    struct compound_next *res = hcalloc(1, sizeof(struct compound_next));

    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);

    if ((res->a_o = build_and_or(lex)) == NULL)
        return NULL;
    if (lex->current_tok->type == TOKEN_PTCOMA)
        res->esp = 1;
    else if (lex->current_tok->type == TOKEN_ESP)
        ;
    else if (lex->current_tok->type == TOKEN_NEWLINE)
        ;
    else
        return NULL;
    lexer_pop(lex);

    res->next = build_compound_next(lex);
    return res;
}
struct compound_list *build_compound_list(struct lexer *lex)
{
    struct compound_list *res = hcalloc(1, sizeof(struct compound_list));

    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);
    if ((res->and_or = build_and_or(lex)) != NULL)
    {
        if (lex->current_tok->type == TOKEN_ESP)
            res->esp = 1;
        else if (lex->current_tok->type == TOKEN_NEWLINE)
            ;
        else if (lex->current_tok->type == TOKEN_PTCOMA)
            ;
        else
            return res;
        lexer_pop(lex);

        res->next = build_compound_next(lex);
        return res;
    }
    return NULL;
}

struct rule_while *build_rule_while(struct lexer *lex)
{
    struct rule_while *res = hcalloc(1, sizeof(struct rule_while));

    if (lex->current_tok->type != TOKEN_WHILE)
        return NULL;

    lexer_pop(lex);
    if ((res->cp_list = build_compound_list(lex)) != NULL)
    {
        if ((res->do_gp = build_do_group(lex)) != NULL)
            return res;

        errx(1, "Missing do_group");
    }
    errx(1, "missing compound_list");
}

 struct rule_until *build_rule_until(struct lexer *lex)
{
     struct rule_until *res = hcalloc(1, sizeof(struct rule_until));
     if (lex->current_tok->type != TOKEN_UNTIL)
         return NULL;

     lexer_pop(lex);
     if ((res->cp_list = build_compound_list(lex)) != NULL)
     {
         if ((res->do_gp = build_do_group(lex)) != NULL)
             return res;

         errx(1, "Missing do_group");
     }
     errx(1, "missing compound_list");
 }

struct rule_if *build_rule_if(struct lexer *lex)
{
    struct rule_if *res = hcalloc(1, sizeof(struct rule_if));

    if (lex->current_tok->type != TOKEN_IF)
        return NULL;
    lexer_pop(lex);

    if ((res->cp_list = build_compound_list(lex)) == NULL)
        errx(1, "Missing compound_list in if");

    if (lex->current_tok->type != TOKEN_THEN)
        errx(1, "then token is missing");
    lexer_pop(lex);

    if ((res->cp_list2 = build_compound_list(lex)) == NULL)
        errx(1, "missing compound_list in if");

    res->else_cl = build_else_clause(lex);

    if (lex->current_tok->type != TOKEN_FI)
        errx(1, "missing fi");
    lexer_pop(lex);
    return res;
}

struct rule_for *build_rule_for(struct lexer *lex)
{
    struct rule_for *res = hcalloc(1, sizeof(struct rule_for));

    if (lex->current_tok->type != TOKEN_FOR)
        return NULL;
    lexer_pop(lex);
    if (lex->current_tok->type != TOKEN_WORDS)
        errx(1, "missing word after for");

    res->word = lexer_pop(lex)->value;
    if (valid_name(res->word) == 0)
        errx(1, "not valid variable for for");
    if (lex->current_tok->type == TOKEN_PTCOMA)
        lexer_pop(lex);
    else
    {
        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);

        if (lex->current_tok->type != TOKEN_WORDS
            || strcmp(lex->current_tok->value, "in") != 0) // TOKEN IN
            errx(1, "missing 'in' in for");
        lexer_pop(lex);
        while (lex->current_tok->type == TOKEN_WORDS)
        {
            res->word_list = hrealloc(res->word_list, ++(res->wl_s) * sizeof(char *) );
            res->word_list[res->wl_s - 1] = lexer_pop(lex)->value;
//            printf("%s", res->word_list[res->wl_s - 1]);
        }
        if (lex->current_tok->type != TOKEN_PTCOMA
            && lex->current_tok->type != TOKEN_NEWLINE)

            errx(1, "not got sep op after word list in for");
    }
    lexer_pop(lex);
    if ((res->do_gp = build_do_group(lex)) == NULL)
        errx(1, "failed to build do group");
    return res;
}

struct else_clause *build_else_clause(struct lexer *lex)
{
    struct else_clause *res = hcalloc(1, sizeof(struct else_clause));

    if (lex->current_tok->type == TOKEN_ELSE)
    {
        lexer_pop(lex);
        if ((res->cp_list = build_compound_list(lex)) != NULL)
            return res;
        errx(1, "Missing compound_list");
    }
    if (lex->current_tok->type == TOKEN_ELIF)
    {
        lexer_pop(lex);

        if ((res->cp_list2 = build_compound_list(lex)) == NULL)
            errx(1, "Missing compound_list after elif");

        if (lex->current_tok->type != TOKEN_THEN)
            errx(1, "'then' token is missing");
        lexer_pop(lex);

        if ((res->cp_list2bis = build_compound_list(lex)) == NULL)
            errx(1, "missing compound_list in elif");

        res->next = build_else_clause(lex);

        return res;
    }
    return NULL;
}

struct do_group *build_do_group(struct lexer *lex)
{
    struct do_group *res = hcalloc(1, sizeof(struct do_group));

    if (lex->current_tok->type != TOKEN_DO)
        return NULL;
    lexer_pop(lex);

    if ((res->cp_list = build_compound_list(lex)) != NULL)
    {
        if (lex->current_tok->type != TOKEN_DONE)
            errx(1, " Missing token done");
        lexer_pop(lex);
        return res;
    }
    errx(1, "Missing compound_list after do for do_group");
}
