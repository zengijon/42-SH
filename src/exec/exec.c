#include "../exec/exec.h"

#include <stdio.h>

#include "../exec_builtins/exec_cmds.h"
#include "../lexer/lexer.h"
#include "../memory/hmalloc.h"
#include "../parser/parser.h"
#include "assert.h"
#include "string.h"
#include "variable_expention.h"

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
    int res;
    if (p->next != NULL)
    {
        res = my_pipe(p->cmd, p->next, ex_l);
        return p->negation == 0 ? res : !res;
    }
    else
    {
        res = exec_command(p->cmd, ex_l);
        return p->negation == 0 ? res : !res;
    }
}

int exec_command(struct command *cmd)
{
    int res = 0;
    for (int i = 0; i < cmd->nb_redir; i++)
    {
        if ((res = exec_redir(cmd->redir[i], ex_l)) != 0)
            return res;
    }
    for (int i = 0; i < cmd->nb_redir2; i++)
    {
        if ((res = exec_redir(cmd->redir2[i], ex_l)) != 0)
            return res;
    }
    if (cmd->s_cmd != NULL)
        return exec_simple_command(cmd->s_cmd);
    if (cmd->sh_cmd != NULL)
        return exec_shell_command(cmd->sh_cmd, ex_l);
    //    if (cmd->fun != NULL)
    //        return exec_fundec(cmd->fun, ex_l);
    while (ex_l->r_l_size-- > 0)
        reinit_redir(&ex_l->r_l[ex_l->r_l_size]);
    ex_l->r_l_size = 0;
    return res;
}

int exec_simple_command(struct simple_command *cmd, struct exec_struct *ex_l)
{
    int res = 0;
    for (int i = 0; cmd->size_elt == 0 && i < cmd->size_pre; ++i)
        exec_redir(cmd->list_pre[i]->redirect, ex_l);
    for (int i = 0; cmd->size_elt == 0 && i < cmd->size_pre; ++i)
        exec_prefix(cmd->list_pre[i], ex_l);
    for (int i = 0; i < cmd->size_elt; ++i)
        exec_redir(cmd->list_elt[i]->redirect, ex_l);
    //        if (cmd->list_elt[i]->redirect != NULL)
    //            if ((res = exec_redir(cmd->list_elt[i]->redirect, ex_l)) != 0)
    //                return res;
    if (cmd->size_elt < 1)
        return res;
    char **list = hcalloc(cmd->size_elt - 1, sizeof(char *));
    for (int i = 1 ; i < cmd->size_elt; ++i)
    {
        list[i - 1] = remove_sep(cmd->list_elt[i]->word, ex_l);
    }
    res = exec_cmds(remove_sep(cmd->list_elt[0]->word, ex_l), cmd->size_elt - 1,
                    list); // Not in this file
    while (ex_l->r_l_size-- > 0)
        reinit_redir(&ex_l->r_l[ex_l->r_l_size]);
    ex_l->r_l_size = 0;
    return res;
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
        return exec_rule_while(cmd->r_w, ex_l);
    if (cmd->r_u != NULL)
        return exec_rule_until(cmd->r_u, ex_l);
    assert(0);
}
//
// int exec_fundec(struct funcdec *cmd, struct exec_struct *ex_l)
//{
//    // a faire
//    if (cmd)
//        return 0;
//    return 0;
//}
//

int exec_redir(struct redirection *r, struct exec_struct *ex_l)
{
    if (r == NULL)
        return 0;
    ex_l->r_l = hrealloc(ex_l->r_l, ++ex_l->r_l_size * sizeof(struct redir));
    if (fnmatch("*>", r->redir_type, 0) == 0
        || fnmatch("<>", r->redir_type, 0) == 0
        || fnmatch(">|", r->redir_type, 0) == 0)
        return simple_redir(strtok(r->redir_type, "><|& "), r->word,
                            &ex_l->r_l[ex_l->r_l_size - 1], "w");
    if (fnmatch("*<", r->redir_type, 0) == 0)
        return simple_redir(strtok(r->redir_type, "><|& "), r->word,
                            &ex_l->r_l[ex_l->r_l_size - 1], "w");
    if (fnmatch("*>&", r->redir_type, 0) == 0)
        return esp_redir(strtok(r->redir_type, "><|& "), r->word,
                         &ex_l->r_l[ex_l->r_l_size - 1], 1);
    if (fnmatch("*<&", r->redir_type, 0) == 0)
        return esp_redir(strtok(r->redir_type, "><|& "), r->word,
                         &ex_l->r_l[ex_l->r_l_size - 1], 0);
    if (fnmatch("*>>", r->redir_type, 0) == 0)
        return append_redir(strtok(r->redir_type, "><|& "), r->word,
                            &ex_l->r_l[ex_l->r_l_size - 1]);
    assert(0);
}

int exec_redir(struct redirection *cmd)
{
    value = remove_sep(value, ex_l);
    int res = 0;
    for (int i = 0; i < ex_l->v_l_size; i++)
        if (strcmp(ex_l->v_l[i].name, name) == 0)
        {
            ex_l->v_l[i].value = value;
            ex_l->v_l[i].value_l = strlen(value);
            return res;
        }
    ex_l->v_l = hrealloc(ex_l->v_l, (++ex_l->v_l_size) * sizeof(char *));
    ex_l->v_l[ex_l->v_l_size - 1].name = name;
    ex_l->v_l[ex_l->v_l_size - 1].value = value;
    ex_l->v_l[ex_l->v_l_size - 1].name_l =
        strlen(ex_l->v_l[ex_l->v_l_size - 1].name);
    ex_l->v_l[ex_l->v_l_size - 1].value_l =
        strlen(ex_l->v_l[ex_l->v_l_size - 1].value);
    return res;
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
    assert(r_w);
    int res = 0;
    while (exec_compound_list(r_w->cp_list, ex_l) == 0)
        res = exec_do_group(r_w->do_gp, ex_l);
    return res;
}

int exec_rule_until(struct rule_until *r_u, struct exec_struct *ex_l)
{
    assert(r_u);
    int res = 0;
    while (exec_compound_list(r_u->cp_list, ex_l) == 0)
        res = exec_do_group(r_u->do_gp, ex_l);
    return res;
}

// int exec_rule_case(struct rule_case *r_c, struct exec_struct *ex_l)
//{
//     if (r_c)
//         return 0;
//     return 0;
// }

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
