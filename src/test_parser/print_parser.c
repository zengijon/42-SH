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
    printf("(list) [");
    if (l != NULL)
    {
        print_and_or(l->a_o);
        print_list_next(l->next);
        if (sep != 0)
            printf("(separator : %d )", l->sep);
    }
    printf("] ");
    return;
}
void print_and_or_next(struct and_or_next *a_o_n)
{
    if (a_o_n != NULL)
    {
        printf("(operator : %d )", a_o_n->op);
        printf("( new line )");
        print_pipeline(a_o_n->pipeline);
        print_and_or_next(a_o_n->next);
    }
    return;
}

void print_and_or(struct and_or *a_o)
{
    printf("(and_or) [");
    if (a_o != NULL)
    {
        print_pipeline(a_o->pipeline);
        print_and_or_next(a_o->next);
    }
    printf("] ");
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
    printf("(pipeline) [ ");
    if (p != NULL)
    {
        if (p->negation != 0)
            printf("( ! )");
        print_command(p->cmd);
        print_pipeline_next(p->next);
    }
    printf("]");
    return;
}

void print_command(struct command *c)
{
    printf("(command) [ ");
    if (c != NULL)
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
    }
    printf("]");
    return;
}

void print_simple_command(struct simple_command *s_c)
{
    printf("(simple_command) [ ");
    if (s_c != NULL)
    {
        for (int i = 0; i < s_c->size_pre; ++i)
            print_prefix(list_pre[i]);
        for (int j = 0; j < s_c->size_elt; ++j)
            print_element(list_elt[j]);
    }
    printf("]");
    return;
}

void print_shell_command(struct shell_command *sh_cmd)
{
    printf("(shell_command) [ ");
    if (sh_cmd != NULL)
    {
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
    }
    printf("]");
    return;
}

void print_funcdec(struct funcdec *f)
{
    printf("(funcdec) [");
    if (f != NULL)
    {
        if (f->funct_name != NULL)
            printf("(function name : %s )", f->funct_name);
        printf("( )");
        printf("( new line )");
        print_shell_command(f->sh_cmd);
    }
    printf("]");
    return;
}

void print_redirection(struct redirection *r)
{
    printf("(redirection) [");
    if (r != NULL)
    {
        if (r->IONUMBER != -1) // a changer dans les fonctions build
            printf("(IONUMBER : %d )", r->IONUMBER);
        printf("(redirect_op : %d )", r->re_op);
        if (r->word != NULL)
            printf("(word : %s )", r->word);
        print_redirection(r->next);
    }
    printf("]");
    return;
}

void print_prefix(struct prefix *p)
{
    printf("(prefix) [");
    if (p != NULL)
    {
        if (p->assignment_word != NULL)
            printf("(assignment word : %s )", p->assignment_word);
        else if (p->redirect)
            print_redirection(p->redirect);
    }
    printf("]");
    return;
}


void print_element(struct element *e)
{
    printf("(element) [");
    if (p != NULL)
    {
        if (p->word != NULL)
            printf("(word : %s )", p->word);
        else if (p->redirect)
            print_redirection(p->redirect);
    }
    printf("]");
    return;
}

void print_compound_next(struct compound_next *c_n)
{
    if (c_n != NULL)
    {
        if (c_n->sep != 0)
            printf("(separator : %d )", c_n->sep);
        printf("( new line )");
        if (c_n->a_o != NULL)
            print_and_or(c_n->a_o);
        print_compound_next(c_n->next);
    }
    return;
}

void print_compound_list(struct compound_list *c_l)
{
    printf("(compound_list) [");
    if (c_l != NULL)
    {
        printf("( new line )");
        if (c_l->and_or != NULL)
            print_and_or(c_l->and_or);
        print_compound_next(c_l->next);
    }
    printf("]");
    return;
}

void print_rule_for(struct rule_for *r_f)
{
    printf("(rule_for) [");
    if (r_f != NULL)
    {
        printf("( for )");
        if (r_f->word != NULL)
            printf("( word : )");
        if (r_f->sep == 1)
            printf("(separator : %d )", r_f->sep);
        else if (r_f->word_list != NULL)
        {
            printf("( new line )");
            printf("( in )");
            printf("(word_list :");
            for (int i = 0; i < r_f->size_word_list; ++i)
                printf(" &s", r_f->word_list[i]);
            printf(" )");
            printf("(separator : %d )", r_f->sep_op);
        }
        printf("( new line )");
        if (r_f->do_gp)
            print_do_group(r_df->do_gp);
    }
    printf("]");
    return;
}

void print_while(struct rule_while *r_w)
{
    printf("(rule while) [");
    if (r_w != NULL)
    {
        printf("( while )");
        if (r_w->cp_list != NULL)
            print_compound_list(r_w->cp_list);
        if (r_w->do_gp != NULL)
            print_do_group(r_w->do_gp);
    }
    printf("]");
    return;
}

void print_until(struct rule_until *r_u)
{
    printf("(rule until) [");
    if (r_u != NULL)
    {
        printf("( until )");
        if (r_u->cp_list != NULL)
            print_compound_list(r_u->cp_list);
        if (r_u->do_gp != NULL)
            print_do_group(r_u->do_gp);
    }
    printf("]");
    return;
}

void print_rule_case(struct rule_case *r_c)
{
    printf("(rule case) [ ");
    if (r_c != NULL)
    {
        printf("( case ) ");
        if (r_c->word != NULL)
            printf("(word : %s ) ", r_c->word);
        printf("( new line ) ");
        printf("( in ) ");
        printf("( new line ) ");
        if (r_c->case_cl != NULL)
            print_case_clause(r_c->case_cl);
        //printf("ESAC"); ???
    }
    printf("]");
    return;
}

void print_rule_if(struct rule_if *r_i)
{
    printf("(rule if) [ ");
    if (r_i != NULL)
    {
        printf("( if ) ");
        if (r_i->cp_list != NULL)
            print_compound_list(r_i->cp_list);
        printf("( then )");
        if (r_i->cp_list2 != NULL)
            print_compound_list(r_i->cp_list2);
        print_else_clause(r_i->else_cl);
        printf("( fi )");
    }
    printf("]");
    return;
}

// void else_clause

void print_do_group(struct do_group *d_g)
{
    printf("(do group) [");
    if (d_g != NULL)
    {
        printf("( do )");
        if (d_g->cp_list != NULL)
            print_compound_list(d_g->cp_list);
        printf("( done )");
    }
    printf("]");
    return;
}

void print_case_clause_bis(struct case_clause_bis *c)
{
    if (c != NULL)
    {
        if (c->is_double_semi == 1)
            printf("( ; )");
        printf("( new line )");
        if (c->case_it != NULL)
            print_case_item(c->case_it);
    }
    return;
}

void print_case_clause(struct case_clause *c_c)
{
    printf("(case_clause) [");
    if (c_c != NULL)
    {
        if (c_c->case_it != NULL)
            print_case_item(c_c->case_it);
        if (c_c->next)
            print_case_clause_bis(c_c->next);
        if (c_c->is_double_semi == 1)
            printf("( ;; )");
        printf("( new line )");
    }
    printf("]");
    return;
}

void print_case_item(struct case_item *c_i)
{
    printf("(case item) [");
    if (c_i != NULL)
    {
        if (c_i->is_open_bracket != 0)
            printf("( ( ");
        if (c_i->word != NULL)
            printf("%s", c_i->word);
        printf(" |");
        for (int i = 0; i < c_i->size_word_list; ++i)
            printf(" %s", c_i->word_list[i]);
        printf(")");
        printf("( new line )");
        print_compound_list(c_i->cp_list);
    }
    printf("]");
    return;
}