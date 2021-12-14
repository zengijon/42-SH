//
// Created by clousty8 on 24/11/2021.
//

#include "parser.h"

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
    else if (lex->current_tok->type == TOKEN_NEWLINE
             || lex->current_tok->type == TOKEN_EOF)
        return res;
    else
        errx(2, "missing separator after list");
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
    else if (lex->current_tok->type == TOKEN_NEWLINE
             || lex->current_tok->type == TOKEN_EOF)
        return res;
    else
        errx(2, "missing separator after list");
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
        errx(2, "Missing pipeline after operator");

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
        errx(2, "Missing cmd after | and new line");
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
            errx(2, "missing command after negation mark '!' ");
    }
    res->next = build_pipeline_next(lex);

    return res;
}

struct command *build_command(struct lexer *lex)
{
    struct command *res = hcalloc(1, sizeof(struct command));
    struct redirection *tmp;
    while ((tmp = build_redirection(lex)) != NULL)
    {
        res->redir = hrealloc(res->redir,
                              ++(res->nb_redir) * sizeof(struct redirection *));
        res->redir[res->nb_redir - 1] = tmp;
    }
    if ((res->fun = build_funcdec(lex)) != NULL)
        ;
    else if ((res->s_cmd = build_simple_command(lex)) != NULL)
        return res;
    else if ((res->sh_cmd = build_shell_command(lex)) != NULL)
        ;
    else if (res->redir != NULL)
        return res;
    else
        return NULL;
    while ((tmp = build_redirection(lex)) != NULL)
    {
        res->redir2 =
            hrealloc(res->redir2, ++(res->nb_redir2) * sizeof(struct prefix *));
        res->redir2[res->nb_redir2 - 1] = tmp;
    }
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
    if (lex->current_tok->type == TOKEN_ACO_OPEN)
    {
        lexer_pop(lex);
        if ((res->c_p = build_compound_list(lex)) == NULL)
            errx(2, "missing compound list between '{}'");
        if (lex->current_tok->type != TOKEN_ACO_CLOSE)
            errx(2, "syntax error : missing '}'");
        lexer_pop(lex);
        return res;
    }
    if (lex->current_tok->type == TOKEN_PA_OPEN)
    {
        lexer_pop(lex);
        res->is_subshell = 1;
        if ((res->c_p = build_compound_list(lex)) == NULL)
            errx(2, "missing compound list between '()'");
        if (lex->current_tok->type != TOKEN_PA_CLOSE)
            errx(2, "syntax error : missing ')'");
        lexer_pop(lex);
        return res;
    }
    //
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
    else
        return NULL;
    return res;
}

struct funcdec *build_funcdec(struct lexer *lex)
{
    struct funcdec *res = hcalloc(1, sizeof(struct funcdec));

    if (lex->current_tok->type == TOKEN_WORDS)
    {
        if (lexer_peek(lex)->type != TOKEN_PA_OPEN)
            return NULL;
        res->funct_name = lexer_pop(lex)->value;
        lexer_pop(lex);
        if (lex->current_tok->type != TOKEN_PA_CLOSE)
            errx(2, "Open parenthethis in weird context");

        lexer_pop(lex);

        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);
        if ((res->sh_cmd = build_command(lex)) == NULL)
            errx(2, "Missing shell cmd after word () new line");
        return res;
    }
    return NULL;
}

struct redirection *build_redirection(struct lexer *lex)
{
    struct redirection *res = hcalloc(1, sizeof(struct funcdec));
    if (lex->current_tok->type != TOKEN_REDIR)
        return NULL;
    res->redir_type = lexer_pop(lex)->value;
    if (lex->current_tok->type != TOKEN_WORDS)
        errx(2, "word exepected after redir");
    res->word = lexer_pop(lex)->value;
    return res;
}

struct prefix *build_prefix(struct lexer *lex)
{
    struct prefix *res = hcalloc(1, sizeof(struct prefix));

    if (lex->current_tok->type != TOKEN_WORDS
        || fnmatch("*=*", lex->current_tok->value, 0) != 0
        || lex->current_tok->value[0] == '=')
        return NULL;
    if (valid_name(lex->current_tok->value) == 0)
        return NULL;
    res->assignment_word = lexer_pop(lex)->value;
    res->redirect = build_redirection(lex);

    return res;
}

struct element *build_element(struct lexer *lex)
{
    struct element *res = hcalloc(1, sizeof(struct element));

    if (lex->current_tok->type != TOKEN_WORDS)
        return NULL;
    res->word = lexer_pop(lex)->value;

    res->redirect = build_redirection(lex);

    return res;
}

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
        return res;
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

        errx(2, "Missing do_group");
    }
    errx(2, "missing compound_list");
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

        errx(2, "Missing do_group");
    }
    errx(2, "missing compound_list");
}

struct rule_if *build_rule_if(struct lexer *lex)
{
    struct rule_if *res = hcalloc(1, sizeof(struct rule_if));

    if (lex->current_tok->type != TOKEN_IF)
        return NULL;
    lexer_pop(lex);

    if ((res->cp_list = build_compound_list(lex)) == NULL)
        errx(2, "Missing compound_list in if");

    if (lex->current_tok->type != TOKEN_THEN)
        errx(2, "then token is missing");
    lexer_pop(lex);

    if ((res->cp_list2 = build_compound_list(lex)) == NULL)
        errx(2, "missing compound_list in if");

    res->else_cl = build_else_clause(lex);

    if (lex->current_tok->type != TOKEN_FI)
        errx(2, "missing fi");
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
        errx(2, "missing word after for");

    res->word = lexer_pop(lex)->value;
    if (valid_name(res->word) == 0)
        errx(2, "not valid variable for for");
    if (lex->current_tok->type == TOKEN_PTCOMA)
        lexer_pop(lex);
    else
    {
        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);

        if (lex->current_tok->type != TOKEN_WORDS
            || strcmp(lex->current_tok->value, "in") != 0) // TOKEN IN
            errx(2, "missing 'in' in for");
        lexer_pop(lex);
        while (lex->current_tok->type == TOKEN_WORDS)
        {
            res->word_list =
                hrealloc(res->word_list, ++(res->wl_s) * sizeof(char *));
            res->word_list[res->wl_s - 1] = lexer_pop(lex)->value;
            //           printf("%s", res->word_list[res->wl_s - 1]);
        }
        if (lex->current_tok->type != TOKEN_PTCOMA
            && lex->current_tok->type != TOKEN_NEWLINE)

            errx(2, "not got sep op after word list in for");
    }
    lexer_pop(lex);
    if ((res->do_gp = build_do_group(lex)) == NULL)
        errx(2, "failed to build do group");
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
        errx(2, "Missing compound_list");
    }
    if (lex->current_tok->type == TOKEN_ELIF)
    {
        lexer_pop(lex);

        if ((res->cp_list2 = build_compound_list(lex)) == NULL)
            errx(2, "Missing compound_list after elif");

        if (lex->current_tok->type != TOKEN_THEN)
            errx(2, "'then' token is missing");
        lexer_pop(lex);

        if ((res->cp_list2bis = build_compound_list(lex)) == NULL)
            errx(2, "missing compound_list in elif");

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
            errx(2, " Missing token done");
        lexer_pop(lex);
        return res;
    }
    errx(2, "Missing compound_list after do for do_group");
}

struct rule_case *build_rule_case(struct lexer *lex)
{
    struct rule_case *res = hcalloc(1, sizeof(struct rule_case));

    if (lex->current_tok->type != TOKEN_CASE)
        return NULL;

    lexer_pop(lex);
    if (lex->current_tok->type == TOKEN_WORDS)
    {
        res->word = lexer_pop(lex)->value;

        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);

        if (lex->current_tok->type != TOKEN_WORDS
            || strcmp("in", lex->current_tok->value) != 0)
            errx(1, "Missing in in case");
        lexer_pop(lex);

        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);

        res->case_cl = build_case_clause(lex);

        if (lex->current_tok->type != TOKEN_ESAC)
            errx(2, "missing esac after case");
        lexer_pop(lex);
        return res;
    }
    errx(1, "Missing word after case");
}

struct case_clause *build_case_clause(struct lexer *lex)
{
    struct case_clause *res = hcalloc(1, sizeof(struct case_clause));

    if ((res->case_it = build_case_item(lex)) == NULL)
        return NULL;

    while (lex->current_tok->type == TOKEN_PTCOMA
           && lexer_peek(lex)->type == TOKEN_PTCOMA)
    {
        lexer_pop(lex);
        lexer_pop(lex);

        while (lex->current_tok->type == TOKEN_NEWLINE)
            lexer_pop(lex);
        res->next =
            hrealloc(res->next, ++res->next_size * sizeof(struct case_item));
        if ((res->next[res->next_size - 1] = build_case_item(lex)) == NULL)
        {
            res->next_size--;
            return res;
        }
    }
    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);
    return res;
}

struct case_item *build_case_item(struct lexer *lex)
{
    struct case_item *res = hcalloc(1, sizeof(struct case_item));

    if (lex->current_tok->type == TOKEN_PA_OPEN)
        lexer_pop(lex); // error handling aproximative

    if (lex->current_tok->type != TOKEN_WORDS)
        return NULL;
    res->word_list = hcalloc(1, sizeof(char *));
    res->word_list[res->w_l_size++] = lexer_pop(lex)->value;
    while (lex->current_tok->type == TOKEN_PIPE)
    {
        lexer_pop(lex);
        res->word_list =
            hrealloc(res->word_list, ++res->w_l_size * sizeof(char *));
        if ((res->word_list[res->w_l_size - 1] = lexer_pop(lex)->value) == NULL)
            errx(2, "missing word after pipe in case item");
    }

    if (lexer_pop(lex)->type != TOKEN_PA_CLOSE)
        errx(2, "missing paren after word list in case item");
    while (lex->current_tok->type == TOKEN_NEWLINE)
        lexer_pop(lex);

    res->cp_list = build_compound_list(lex);
    return res;
}
