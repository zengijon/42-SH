//
// Created by clousty8 on 26/11/2021.
//
#include "stdio.h"
#include "print_parser.h"
#include "../parser/parser.h"
#include "../struct/grammar_struct.h"

void print_list_next(struct list_next *l)
{
    if (l == NULL)
        return;
    if (sep != 0)
        printf("(separator : %d )", l->sep);
    print_and_or(l->a_o);
    print_list_next(l->next);
}

void print_list(struct list *l)
{
    if (l == NULL)
        return;
    printf("(list) [");
    print_and_or(l->a_o);
    print_list_next(l->next);
    if (sep != 0)
        printf("(separator : %d )", l->sep);
    printf("] ");
}
void print_and_or_next(struct and_or_next *l)
{
    if (l == NULL)
        return;
    printf("(operator : %d )", l->op);
    printf("( new line )");
    print_pipeline(l->pipeline);
    print_and_or_next(l->next);
    return;
}

void print_and_or(struct and_or *a_o)
{
    if (l == NULL)
        return;
    printf("(and-or) [");
    print_pipeline(a_o->pipeline);
    print_and_or_next(a_o->next);
    printf("] ");
}

void print_pipeline_next(struct pipeline_next *p)
{
    if (p->cmd != NULL)
    {
        printf("( | )");
        printf("( new line )");
        print_command(p->command);
        print_pipeline_next(p->next);
    }
    return;
}

void print_pipeline(struct pipeline *p)
{
    if (p == NULL)
        return;

    if (p->negation != 0)
        printf("( ! )");
    print_command(p->cmd);
    print_pipeline_next(p->next);
    return;
}

void print_command(struct command *c)
{
    if (c->cmd != NULL)
        print_command(c->choose.cmd);
    else if (c->sh_cmd != NULL)
    {
        print_shell_command(c->choose.sh_cmd);
        print_redirection(c->redir);
    }
    else if (c->fun)
    {
        print_funcdec(c->choose.fun);
        print_redirection(c->redir);
    }
    return;
}

void print_simple_command(struct simple_command *s_c)
{
    for (int i = 0; i < s_c->size_pre; ++i)
        print_prefix(list_pre[i]);
    for (int j = 0; j < s_c->size_elt; ++j)
        print_element(list_elt[j]);
    return;
}

void