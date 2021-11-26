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
    print_list(l->next);
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

    struct list *temp = l->next;
    while (temp != NULL)
    {
        print_and_or_next(temp);
        temp = temp->next;
    }
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

void print_pipeline(struct pipeline *p)
{
    if (p == NULL)
        return;

    if (p->negation != 0)
        printf("( ! )");
    print_command(p->cmd);
    if (p->pipe_l != NULL)
    {
        printf("( | )");
        printf("( new line )");

    }
    return;
}