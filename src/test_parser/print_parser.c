//
// Created by armand on 26/11/2021.
//
#include "stdio.h"
#include "print_parser.h"

void print_list_next(struct list_next *l)
{
    if (l == NULL)
        return;
    if (l->sep != 0)
        printf("(separator : %d ) ", l->sep);
    print_and_or(l->a_o);
    print_list_next(l->next);
}

void print_list(struct list *l)
{
    printf("(list) [ \n");
    if (l != NULL)
    {
        print_and_or(l->a_o);
        print_list_next(l->next);
        if (l->sep != 0)
            printf("(separator : %d ) ", l->sep);
    }
    printf("\n]");
}
void print_and_or_next(struct and_or_next *a_o_n)
{
    if (a_o_n != NULL)
    {
        printf("\n(operator : ");
        if (a_o_n->op == AND)
            printf("&& ");
        else if (a_o_n->op == OR)
            printf("|| ");
        else
            printf(")\n ");

        //printf("( new line ) ");
        print_pipeline(a_o_n->pipeline);
        print_and_or_next(a_o_n->next);
    }
}

void print_and_or(struct and_or *a_o)
{
    if (a_o != NULL)
        printf(" (and_or) [ ");
    {
        print_pipeline(a_o->pipeline);
        print_and_or_next(a_o->next);
        printf(" ]");
    }
}

void print_pipeline_next(struct pipeline_next *p)
{
    if (p != NULL)
    {
        printf(" ( | ) "); // ??? checker la grammaire plus tard
        //printf("( new line ) ");
        print_command(p->cmd);
        print_pipeline_next(p->next);
    }
}

void print_pipeline(struct pipeline *p)
{
    if (p != NULL)
    {
        printf(" (pipeline) [ ");
        if (p->negation != 0)
            printf(" ( ! ) ");
        print_command(p->cmd);
        print_pipeline_next(p->next);
        printf("]");
    }
}

void print_command(struct command *c)
{
    if (c != NULL)
    {
        printf(" (command) [ ");
        if (c->s_cmd != NULL)
            print_simple_command(c->s_cmd);
        else if (c->sh_cmd != NULL)
        {
            print_shell_command(c->sh_cmd);
            //print_redirection(c->redir);
        }
        //else if (c->fun)
        //{
            //print_funcdec(c->fun);
            //print_redirection(c->redir);
        //}
        printf("]");
    }
}

void print_simple_command(struct simple_command *s_c)
{
    if (s_c != NULL)
    {
        printf(" \n\t(simple_command) [ ");
        //for (int i = 0; i < s_c->size_pre; ++i)
            //print_prefix(s_c->list_pre[i]);
        for (int j = 0; j < s_c->size_elt; ++j)
            print_element(s_c->list_elt[j]);
        printf("]");
    }
}

void print_shell_command(struct shell_command *sh_cmd)
{
    if (sh_cmd != NULL)
    {
        printf(" (shell_command) [ ");
        if (sh_cmd->c_p != NULL)
            print_compound_list(sh_cmd->c_p);
        //else if (sh_cmd->r_f != NULL)
            //print_rule_for(sh_cmd->r_f);
        //else if (sh_cmd->r_w != NULL)
            //print_rule_while(sh_cmd->r_w);
        //else if (sh_cmd->r_u != NULL)
            //print_rule_until(sh_cmd->r_u);
       // else if (sh_cmd->r_c != NULL)
            //print_rule_case(sh_cmd->r_c);
        else if (sh_cmd->r_i != NULL)
            print_rule_if(sh_cmd->r_i);
        printf("]");
    }
}

//void print_funcdec(struct funcdec *f)
//{
//    printf("(funcdec) [");
//    if (f != NULL)
//    {
//        if (f->funct_name != NULL)
//            printf("(function name : %s )", f->funct_name);
//        printf("( )");
//        printf("( new line )");
//        print_shell_command(f->sh_cmd);
//    }
//    printf("]");
//}

//void print_redirection(struct redirection *r)
//{
//    printf("(redirection) [");
//    if (r != NULL)
//    {
//        if (r->IONUMBER != -1) // a changer dans les fonctions build
//            printf("(IONUMBER : %d )", r->IONUMBER);
//        printf("(redirect_op : %d )", r->re_op);
//        if (r->word != NULL)
//            printf("(word : %s )", r->word);
//        print_redirection(r->next);
//    }
//    printf("]");
//}
//
//void print_prefix(struct prefix *p)
//{
//    printf("(prefix) [");
//    if (p != NULL)
//    {
//        if (p->assignment_word != NULL)
//            printf("(assignment word : %s )", p->assignment_word);
//        else if (p->redirect)
//            print_redirection(p->redirect);
//    }
//    printf("]");
//}


void print_element(struct element *e)
{
    if (e != NULL)
    {
        printf(" (element) [ ");
        if (e->word != NULL)
            printf("(word : %s ) ", e->word);
        //else if (e->redirect)
            //print_redirection(e->redirect);
        printf("]");
    }
}

void print_compound_next(struct compound_next *c_n)
{
    if (c_n != NULL)
    {
        if (c_n->sep != 0)
            printf(" (separator : %d ) ", c_n->sep);
        //printf("( new line ) ");
        if (c_n->a_o != NULL)
            print_and_or(c_n->a_o);
        print_compound_next(c_n->next);
    }
}

void print_compound_list(struct compound_list *c_l)
{
    if (c_l != NULL)
    {
        printf(" (compound_list) [ ");
        //printf("( new line ) ");
        if (c_l->and_or != NULL)
            print_and_or(c_l->and_or);
        print_compound_next(c_l->next);
        printf("]");
    }
}

//void print_rule_for(struct rule_for *r_f)
//{
//    printf("(rule_for) [");
//    if (r_f != NULL)
//    {
//        printf("( for )");
//        if (r_f->word != NULL)
//            printf("( word : )");
//        if (r_f->sep_op == Semi)
//            printf("(separator : %d )", r_f->sep_op);
//        else if (r_f->word_list != NULL)
//        {
//            printf("( new line )");
//            printf("( in )");
//            printf("(word_list :");
//            //for (int i = 0; i < r_f->size_word_list; ++i)
//                //printf(" &s", r_f->word_list[i]);
//            printf(" )");
//            printf("(separator : %d )", r_f->sep_op);
//        }
//        printf("( new line )");
//        if (r_f->do_gp)
//            print_do_group(r_f->do_gp);
//    }
//    printf("]");
//}

//void print_while(struct rule_while *r_w) // never used normal
//{
//    printf("(rule while) [");
//    if (r_w != NULL)
//    {
//        printf("( while )");
//        if (r_w->cp_list != NULL)
//            print_compound_list(r_w->cp_list);
//        if (r_w->do_gp != NULL)
//            print_do_group(r_w->do_gp);
//    }
//    printf("]");
//}

//void print_until(struct rule_until *r_u) // never used normal
//{
//    printf("(rule until) [");
//    if (r_u != NULL)
//    {
//        printf("( until )");
//        if (r_u->cp_list != NULL)
//            print_compound_list(r_u->cp_list);
//        if (r_u->do_gp != NULL)
//            print_do_group(r_u->do_gp);
//    }
//    printf("]");
//}

//void print_rule_case(struct rule_case *r_c)
//{
//    printf("(rule case) [ ");
//    if (r_c != NULL)
//    {
//        printf("( case ) ");
//        if (r_c->word != NULL)
//            printf("(word : %s ) ", r_c->word);
//        printf("( new line ) ");
//        printf("( in ) ");
//        printf("( new line ) ");
//        if (r_c->case_cl != NULL)
//            print_case_clause(r_c->case_cl);
//        //printf("ESAC"); ???
//    }
//    printf("]");
//}

void print_rule_if(struct rule_if *r_i)
{
    if (r_i != NULL)
    {
        printf(" (rule if) [ \n");
        printf("( if ) ");
        if (r_i->cp_list != NULL)
            print_compound_list(r_i->cp_list);
        printf("\n");
        printf("( then ) ");
        if (r_i->cp_list2 != NULL)
            print_compound_list(r_i->cp_list2);
        printf("\n");
        print_else_clause(r_i->else_cl);
        printf("( fi ) ");
        printf("]");
    }
}

void print_else_clause(struct else_clause *e_l)
{
    if (e_l == NULL)
        return;
    printf(" (else_clause) [ ");
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
    printf("]");
}


//void print_do_group(struct do_group *d_g)
//{
//    printf("(do group) [");
//    if (d_g != NULL)
//    {
//        printf("( do )");
//        if (d_g->cp_list != NULL)
//            print_compound_list(d_g->cp_list);
//        printf("( done )");
//    }
//    printf("]");
//}

//void print_case_clause_bis(struct case_clause_bis *c)
//{
//    if (c != NULL)
//    {
//        if (c->is_double_semi == 1)
//            printf("( ; )");
//        printf("( new line )");
//        if (c->case_it != NULL)
//            print_case_item(c->case_it);
//    }
//}
//
//void print_case_clause(struct case_clause *c_c)
//{
//    printf("(case_clause) [");
//    if (c_c != NULL)
//    {
//        if (c_c->case_it != NULL)
//            print_case_item(c_c->case_it);
//        if (c_c->next)
//            print_case_clause_bis(c_c->next);
//        if (c_c->is_double_semi == 1)
//            printf("( ;; )");
//        printf("( new line )");
//    }
//    printf("]");
//}

//void print_case_item(struct case_item *c_i)
//{
//    printf("(case item) [");
//    if (c_i != NULL)
//    {
//        if (c_i->is_open_bracket != 0)
//            printf("( ( ");
//        if (c_i->word != NULL)
//            printf("%s", c_i->word);
//        printf(" |");
//        //for (int i = 0; i < c_i->size_word_list; ++i)
//            //printf(" %s", c_i->word_list[i]);
//        printf(")");
//        printf("( new line )");
//        print_compound_list(c_i->cp_list);
//    }
//    printf("]");
//}
