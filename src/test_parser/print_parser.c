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
    if (l != NULL)
    {
        printf("(operator : %d )", l->op);
        printf("( new line )");
        print_pipeline(l->pipeline);
        print_and_or_next(l->next);
    }
    return;
}

void print_and_or(struct and_or *a_o)
{
    if (l != NULL)
    {
        printf("(and-or) [");
        print_pipeline(a_o->pipeline);
        print_and_or_next(a_o->next);
        printf("] ");
    }
    return;
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
    if (p != NULL)
    {
        printf("(pipeline) [ ");
        if (p->negation != 0)
            printf("( ! )");
        print_command(p->cmd);
        print_pipeline_next(p->next);
        printf("]");
    }
    return;
}

void print_command(struct command *c)
{
    if (c != NULL)
    {
        printf("(command) [ ");
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
        printf("]");
    }
    return;
}

void print_simple_command(struct simple_command *s_c)
{
    if (s_c != NULL)
    {
        printf("(simple_command) [ ");
        for (int i = 0; i < s_c->size_pre; ++i)
            print_prefix(list_pre[i]);
        for (int j = 0; j < s_c->size_elt; ++j)
            print_element(list_elt[j]);
        printf("]");
    }
    return;
}

void print_shell_command(struct shell_command *sh_cmd)
{
    if (sh_cmd != NULL)
    {
        printf("(shell_command) [ ");
        if (sh_cmd->c_p != NULL)
            print_compound_list(sh_cmd->c_p);
        else if (sh_cmd->r_f != NULL)
            print_rule_for(sh_cmd->r_f);
        else if (sh_cmd->r_w != NULL)
            print_rule_while(sh_cmd->r_w);
        else if (sh_cmd->r_u != NULl)
            print_rule_until(sh_cmd->r_u);
        else if (sh_cmd->r_c != NULL)
            print_rule_case(sh_cmd->r_c);
        else if (sh_cmd->r_i != NULL)
            print_rule_if(sh_cmd->r_i);
        printf("]");
    }
    return;
}

void print_funcdec(struct funcdec *f)
{
    if (f != NULL)
    {
        printf("(funcdec) [");
        if (f->funct_name != NULL)
            printf("(function name : %s )", f->funct_name);
        printf("( )");
        printf("( new line )");
        print_shell_command(f->sh_cmd);
        printf("]");
    }
    return;
}

void print_redirection(struct redirection *r)
{
    if (r != NULL)
    {
        printf("(redirection) [");
        if (r->IONUMBER != -1) // a changer dans les fonctions build
            printf("(IONUMBER : %d )", r->IONUMBER);
        printf("(redirect_op : %d )", r->re_op);
        printf("(word : %d )", r->word);
        print_redirection(r->next);
        printf("]");
    }
    return;
}

void print_prefix(struct prefix *p)
{
    if ()
}

void print_else_clause(struct else_clause *e_l)
{
    printf("(else_clause) [ ");
    if (e_l == NULL)
        return;
    if (e_l->cp_list != NULL)
    {
        printf( "(else) ");
        print_compound_list(e_l->cp_list);
        return;
    }
    printf( "(elif) ");

    print_compound_list(e_l->cp_list2);
    print_compound_list(e_l->cp_list2bis);

    if (e_l->next)
        print_else_clause(e_l->next);
}
printf("] ");
}