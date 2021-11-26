//
// Created by clousty8 on 24/11/2021.
//

#include "../struct/grammar_struct.h"
#include "parser.h"
#include "../memory/hmalloc.h"

struct list_next *build_list_next(struct lexer *lex)
{
    struct list_next* res = hcalloc(1, sizeof(struct list_next));
    if (/*lexer | seek | reçu != separtor */ )
        return NULL;
    res->sep = 0; //lexer | pop | attendu : separtor
    res->next = build_list(lex);
    if (res->next == NULL)
        return NULL;
    return res;
}

struct list *build_list(char *buffer)
{
    struct list* res = hcalloc(1, sizeof(struct list));

    res->a_o = build_and_or(lex);
    if (res->a_o == NULL)
        return NULL;
    if (/*lexer | seek | reçu == separtor */ )
        res->sep = 0; //lexer | pop | attendu : separtor
    return res;
}

struct and_or_next *build_and_or_next(struct lexer *lex)
{
    struct and_or_next *res = hcalloc(1, sizeof(struct and_or_next));
    if (/*lexer | seek | reçu != operator */)
        return NULL;
    res->op = 0; /*lexer | pop | attendu : separtor */
    while (/*lexer | seek | reçu == '/n' */)
        /*lexer | pop | linebreak '\n' */;
    res->next = build_and_or(lex);
    if (res->next == NULL)
        errx(1, "missing expression after operator");
    return res;
}

struct and_or *build_and_or(struct lexer *lex)
{
    struct and_or *res = hcalloc(1, sizeof(struct and_or));
    res->pipeline = build_pipeline(lex, 1);
    if (res->pipeline == NULL)
        return NULL;

    res->next = build_and_or_next(lex);
    return res;
}

struct pipeline *build_pipeline(struct lexer *lex, int flag)
{
    struct pipeline *res = hcalloc(1, sizeof(struct pipeline));
    if (flag == 1 && /*lexer | seek | reçu == '!' */)
    {
        // lexer | pop
        res->negation = 1;
    }
    res->cmd = build_command(lex);
    if (res->command == NULL)
    {
        if (res->negation == 0)
            return NULL;
        else
            errx(1, "missing command after negation mark '!' ");
    }
    if (/*lexer | seek | reçu == '|' */)
    {
        // lexer | pop
        while (/*lexer | seek | reçu == '\n' */)
            /*lexer | pop | linebreak '\n' */;
        res->pipe_l = build_pipeline(lex, 0);
    }
    return res;
}

struct command *build_command(struct lexer *lex)
{
    struct command *res = hcalloc(1, sizeof(struct command));
    if ((res->choose.cmd = build_simple_command(lex)) != NULL)
        ;
    else if ((res->choose.sh_cmd = build_shell_command(lex)) != NULL)
        ;
    else if ((res->choose.fun = build_funcdec(lex)) != NULL)
        ;
    else
        return NULL;
    res->redir = build_redirection(lex);
    return res;
}

struct simple_command *build_simple_command(struct lexer *lex)
{
    struct simple_command *res = hcalloc(1, sizeof(struct simple_command));
    struct prefix *tmp = NULL;
    while ((tmp = build_prefix(lex)) != NULL)
    {
        res->list_pre = hrealloc(res->list_pre, ++(res->size_pre) * sizeof(struct prefix *));
        res->list_pre[res->size_pre - 1] = tmp;
    }

    struct element tmp2= NULL;
    while ((tmp2 = build_element(lex)) != NULL)
    {
        res->list_elt = hrealloc(res->list_elt, ++(res->size_elt) * sizeof(struct element *));
        res->list_elt[res->size_elt - 1] = tmp;
    }

    if (res->list_pre == NULL && res->list_elt == NULL)
        return NULL;
    return res;
}

union shell_command *build_shell_command(struct lexer *lex, int flag)
{
    struct shell_command *res = hcalloc(1, sizeof(struct shell_command));

    if (/* lexer | seek | reçu == '{'*/)
    {
        /* lexer | pop */
        res->c_p = build_compound_list(lex);
        if (res->c_p == NULL)
            errx(1, "missing compound list between '{}'");
        if (/* lexer | pop | reçu != '}' */)
            errx(1, "syntax error : missing '}'");
    }

    if (/* lexer | seek | reçu == '(' */)
    {
        /* lexer | pop */
        res->c_p = build_compound_list(lex);
        if (res->c_p == NULL)
            errx(1, "missing compound list between '()'");
        if (/* lexer | pop | reçu != ')' */)
            errx(1, "syntax error : missing ')'");
    }

    if ((res->r_f = build_rule_for(lex)) != NULL)
        ;
    else if ((res->r_w = build_rule_while(lex)) != NULL)
        ;
    else if ((res->r_u = build_rule_until(lex)) != NULL)
        ;
    else if ((res->r_c = build_rule_case(lex)) != NULL)
        ;
    else if ((res->r_i = build_rule_if(lex)) != NULL)
        ;

    return res;
}

struct funcdec *build_funcdec(struct lexer *lex)
{
    struct funcdec *res = hcalloc(1, sizeof(struct funcdec));

    if ( /* lexer | seek | reçu == word */ && /* lexer | peek | reçu == '(' */ && /* lexer | seek + 1 | reçu == ')' */)
        res->funct_name = NULL; // lexer | pop |
    while (/* lexer | seek | reçu == word */)
        /* lexer | pop */;
    res->sh_cmd = build_shell_command(lex);
    return res;
}

struct redirection *build_redirection(struct lexer *lex)
{
    struct redirection *res = hcalloc(1, sizeof(struct funcdec));
    if (/* lexer | seek | reçu == io_nb */)
        res->IONUMBER = 0; //lexer | pop
    if (/* lexer | seek | redir != op */)
        return NULL;
    res->re_op = 0; // lexer | pop
    if (/* lexer | pop | reçu != word  */)
        errx (1, "missing file name to redirect to");
    res->next = build_redirection();
    return res;
}

struct prefix *build_prefix(struct lexer *lex)
{
    struct prefix *res = hcalloc(1, sizeof(struct prefix));

    if (/*lexer | seek | attendu : char *assignement_word*/)
    {
        res->assignment_word = ""; /* lexer | pop | attendu : char * word*/
        return res;
    }

    res->redirect = build_redirection(lex);
    if (res->redirect == NULL)
        return NULL;
    return res;
}

struct element *build_element(struct lexer *lex)
{
    struct element *res = hcalloc(1, sizeof(struct element));

    if (/*lexer | seek | attendu : char *word*/)
    {
        res->word =  ""; /* lexer | pop | attendu : char * word*/
        return res;
    }

    res->redirect = build_redirection(lex);
    if (res->redirect == NULL)
        return NULL;
    return res;
}

static struct compound_next *build_compound_next(struct lexer *lex) // pb * (a faire a l'infini)
{
    struct compound_next *res = hcalloc(1, sizeof(struct compound_next));

    if (/*lexer | seek | attendu : separator*/)
    {
        res->sep = 0; /*lexer |pop */
        while (/* lexer | seek | reçu == \n*/)
            ; /* lexer | pop */

        res->a_o = build_and_or(lex);
        if (res->a_o == NULL)
            return res;

        res->next = build_compound_next(lex);
        return res;
    }
    return NULL;
}

struct compound_list *build_compound_list(struct lexer *lex)
{
    struct compound_list *res = hcalloc(1, sizeof(struct compound_list));

    while (/*lexer | seek | attendu : \n*/)  //possibly problematic pop
        ;/*lexer | pop */

    res->and_or *temp = build_and_or(lex);

    if (res->and_or != NULL)
    {
        struct compound_next *next = build_compound_next(lex);
    }
    return NULL;
}

struct rule_if *build_rule_if(struct lexer *lex)
{
    struct rule_if *res = hcalloc(1, sizeof(struct rule_if));

    if (/* lexer | seek | attendu != if*/)
        return NULL;
    // lexer | pop

    res->cp_list = build_compound_list(lex);

    if (res->cp_list == NULL)
        errx(1, "Missing compound_list in if");

    if (/*lexer |seek | reçu != then */)
        errx(1, "then token is missing");
    // lexer | pop

    if ((res->cp_list2 = build_compound_list(lex) == NULL))
        errx(1, "missing compound_list in if");

    res->else_cl = build_else_clause(lex);

    if (/* lexer | seek | if != token fi*/)
        errx(1, "missing fi");
    // lexer | pop
    return res;
}

struct else_clause *build_else_clause(struct lexer *lex)
{
    struct else_clause *res = hcalloc(1, sizeof(union else_clause));

    if (/* lexer |seek |attendu : else */)
    {
        if ((res->cp_list = build_compound_list(lex) != NULL))
            return res;
        errx(1, "Missing compound_list");
    }
    if (/*lexer |seek | attendu : elif*/)
    {
        res->elif.cp_list2 = build_compound_list(lex);

        if (res->elif.cp_list2 == NULL)
            errx(1, "Missing compound_list in if");

        if (/*lexer |seek | reçu != then */)
            errx(1, "then token is missing");
        // lexer | pop

        if ((res->elif.cp_list2bis = build_compound_list(lex) == NULL))
            errx(1, "missing compound_list in if");

        res->elif.next = build_else_clause(lex);

        return res;
    }
    return NULL;
}