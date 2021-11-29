#include "../parser/parser.h"
#include "../struct/fifo_lex.h"
#include "../struct/grammar_struct.h"
#include "assert.h"

int exec_simple_command(struct simple_command *cmd)
{
    int res = 0;
    // gerer prefix
    for (int i = 0; i < cmd->size_elt; ++i)
        if (cmd->list_elt[i]->redirect != NULL)
            if ((res = exec_redir(cmd->list_elt[i]->redirect)) != 0)
                return res;
    for (int i = 0; i < cmd->size_elt; ++i)
        printf("%s",cmd->list_elt[i]->word);
    return 0;
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
    return 0;
}

int exec_redir(struct redirection *cmd)
{
    // a faire
    return 0;
}

int exec_cmd(struct command *cmd)
{
    if (cmd->redir != NULL)
    {
        int res = exec_redir(cmd->redir)
        if (res != 0)
            return res;
    }
    if (cmd->cmd != NULL)
        return exec_simple_command(cmd->cmd);
    if (cmd->sh_cmd != NULL)
        return exec_shell_command(cmd->sh_cmd);
    if (cmd->fun != NULL)
        return exec_fundec(cmd->fun);
    assert(0);
}

int exec_pipeline_next(struct pipeline_next *p) //argmument possibly missing
{
    if (p->next != NULL)
    {
        //pipe exec
    }

    int res = exec_command(p->cmd);
    if (p->next == NULL)
        return res;
    else
        return exec_pipeline_next(p->next); //argmument possibly missing

}

int exec_pipeline(struct pipeline *p)
{
    if (p->next != NULL)
    {
        //pipe exec
    }

    int res = exec_command(p->cmd);
    if (p->next == NULL)
        return res && p->negation;
    else
        return exec_pipeline_next(p->next) && p->negation; //argmument possibly missing
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
        return exec_and_or_next(andor->next, res);
}

int exec_and_or(struct and_or *a_o)
{
    int res = exec_pipeline(a_o->pipeline);
    if (a_o->next == NULL)
        return res;
    else
        return exec_and_or_next(andor->next, res);
}

int exec_list_next(struct list_next *l_n)
{
    int res = exec_and_or(l_n->a_o);
    if (l_n->next == NULL)
        return res;
    else
        return = exec_list_next(l_n->next);
    // handel sep
}

int exec_list(struct list *l)
{
    assert(l != NULL);
    int res = exec_and_or(l->a_o) if (l->next == NULL) return res;
    else return = exec_list_next(l->next);
    // gerer sep
}

// not really usefull
int exec(char *buffer)
{
    struct list *begin = build_list(buffer);
    if (begin == NULL)
        return 0;
    return exec_list(begin);
}