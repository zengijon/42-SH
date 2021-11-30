#include "../exec/exec.h"

#include <stdio.h>

#include "../buItlin/echo/echo.h"
#include "../memory/hmalloc.h"
#include "../parser/parser.h"
#include "assert.h"
#include "string.h"

int exec_list_next(struct list_next *l_n)
{
    int res = exec_and_or(l_n->a_o);
    if (l_n->next == NULL)
        return res;
    else
        return exec_list_next(l_n->next);
    // handel sep
}

int exec_list(struct list *l)
{
    assert(l != 0);
    int res = exec_and_or(l->a_o);
    if (l->next == NULL)
        return res;
    else
        return exec_list_next(l->next);
    // gerer sep
}

int exec_and_or_next(struct and_or_next *a_o, int p_res)
{
    int res;
    if (a_o->op == AND)
        res = p_res || exec_pipeline(a_o->pipeline);
    else
        res = p_res && exec_pipeline(a_o->pipeline);

    if (a_o->next == NULL)
        return res;
    else
        return exec_and_or_next(a_o->next, res);
}

int exec_and_or(struct and_or *a_o)
{
    int res = exec_pipeline(a_o->pipeline);
    if (a_o->next == NULL)
        return res;
    else
        return exec_and_or_next(a_o->next, res);
}

int exec_pipeline_next(struct pipeline_next *p) // argmument possibly missing
{
    if (p->next != NULL)
    {
        // pipe exec
    }

    int res = exec_command(p->cmd);
    if (p->next == NULL)
        return res;
    else
        return exec_pipeline_next(p->next); // argmument possibly missing
}

int exec_pipeline(struct pipeline *p)
{
    if (p->next != NULL)
    {
        // pipe exec
    }

    int res = exec_command(p->cmd);
    if (p->next != NULL)
        res = exec_pipeline_next(p->next);     // argmument possibly missing
    return p->negation == 1 ? ! res : res ;
}

int exec_command(struct command *cmd)
{
    if (cmd->redir != NULL)
    {
        int res = exec_redir(cmd->redir);
        if (res != 0)
            return res;
    }
    if (cmd->s_cmd != NULL)
        return exec_simple_command(cmd->s_cmd);
    if (cmd->sh_cmd != NULL)
        return exec_shell_command(cmd->sh_cmd);
    if (cmd->fun != NULL)
        return exec_fundec(cmd->fun);
    assert(0);
}

int exec_simple_command(struct simple_command *cmd)
{
    int res = 0;
    // gerer prefix
    for (int i = 0; i < cmd->size_elt; ++i)
        if (cmd->list_elt[i]->redirect != NULL)
            if ((res = exec_redir(cmd->list_elt[i]->redirect)) != 0)
                return res;
    if (cmd->size_elt < 1)
        return res;
    char **list = hcalloc(cmd->size_elt - 1, sizeof(char *));
    for (int i = 1 ; i < cmd->size_elt; ++i)
    {
        list[i - 1] = cmd->list_elt[i]->word;
    }
    return exec_command_line(cmd->list_elt[0]->word, cmd->size_elt - 1, list);
}

int exec_shell_command(struct shell_command *cmd)
{
    if (cmd->c_p != NULL)
        return exec_compound_list(cmd->c_p);
    if (cmd->r_c != NULL)
        return exec_rule_case(cmd->r_c);
    if (cmd->r_f != NULL)
        return exec_rule_for(cmd->r_f);
    if (cmd->r_i != NULL)
        return exec_rule_if(cmd->r_i);
    if (cmd->r_w != NULL)
        return exec_rule_while(cmd->r_w);
    if (cmd->r_u != NULL)
        return exec_rule_until(cmd->r_u);
    assert(0);
}

int exec_fundec(struct funcdec *cmd)
{
    // a faire
    if (cmd)
        return 0;
    return 0;
}

int exec_redir(struct redirection *cmd)
{
    // a faire
    if (cmd)
        return 0;
    return 0;
}

int exec_prefix(struct prefix *pre)
{
    if (pre)
        return 0;
    return 0;
}

int exec_element(struct element *elt)
{
    if (elt)
        return 0;
    return 0;
}

int exec_compound_next(struct compound_next *cp_list)
{
    int res = 0;
    if (cp_list->a_o != NULL)
        res = exec_and_or(cp_list->a_o);
    if (cp_list->next == NULL)
        return res;
    else
        return exec_compound_next(cp_list->next);
}

// on ne gere pas le && et ||
int exec_compound_list(struct compound_list *cp_list)
{
    int res = exec_and_or(cp_list->and_or);
    if (cp_list->next == NULL)
        return res;
    else
        return exec_compound_next(cp_list->next);
}

int exec_rule_for(struct rule_for *r_f)
{
    if (r_f)
        return 0;
    return 0;
}

int exec_rule_while(struct rule_while *r_w)
{
    if (r_w)
        return 0;
    return 0;
}

int exec_rule_until(struct rule_until *r_u)
{
    if (r_u)
        return 0;
    return 0;
}

int exec_rule_case(struct rule_case *r_c)
{
    if (r_c)
        return 0;
    return 0;
}

int exec_rule_if(struct rule_if *r_i)
{
    if (exec_compound_list(r_i->cp_list) == 0)
        return exec_compound_list(r_i->cp_list2);
    return exec_else_clause(r_i->else_cl);
}

int exec_else_clause(struct else_clause *e_c)
{
    if (e_c == NULL)
        return 0;
    if (e_c->cp_list)
        return exec_compound_list(e_c->cp_list);
    if (exec_compound_list(e_c->cp_list2) == 0)
        return exec_compound_list(e_c->cp_list2bis);
    return exec_else_clause(e_c->next);
}

int exec_do_group(struct do_group *do_gp)
{
    if (do_gp)
        return 0;
    return 0;
}

int exe_case_clause(struct case_clause *c_c)
{
    if (c_c)
        return 0;
    return 0;
}

int exec_case_item(struct case_item *c_i)
{
    if (c_i)
        return 0;
    return 0;
}
