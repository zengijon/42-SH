#include "../struct/grammar_struct.h"
#include "../struct/fifo_lex.h"
#include "../parser/parser.h"



int exec_simple_command(struct simple_command *cmd)
{
    //gerer prefix
    //microshell
    return 0;
}

int exec_shell_command(struct shell_command *cmd)
{
    if (cmd->c_p != NULL)
        return exec_c_p(cmd->c_p);
    if (cmd->r_c != NULL)
        return exec_r_c(cmd->r_c);
    if (cmd->r_f != NULL)
        return exec_r_f(cmd->r_f);
    if (cmd->r_i != NULL)
        return exec_r_i(cmd->r_i);
    if (cmd->r_w != NULL)
        return exec_r_w(cmd->r_w);
    if (cmd->r_u != NULL)
        return  exec_r_u(cmd->r_u);
    return 1;
}

int exec_fundec(struct funcdec *cmd)
{
    //a faire
    return 0;
}

int exec_redir(struct redirection *cmd)
{
    //a faire
    return 0;
}

int exec_cmd(struct command *cmd)
{
    if (cmd->cmd != NULL)
        return exec_simple_command(cmd->cmd);
    if (cmd->sh_cmd != NULL)
        return exec_shell_command(cmd->sh_cmd);
    if (cmd->fun != NULL)
        return exec_fundec(cmd->fun);
    if (cmd->redir != NULL)
        return exec_redir(cmd->redir);
    return 1;
}

int exec_pipeline_next(struct pipeline_next *pipenext)
{
    struct pipeline_next *tmp = pipenext;
    while (tmp != NULL)
    {
        if (exec_cmd(tmp->cmd) != 0)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

int exec_pipeline(struct pipeline *pipe)
{
    if (pipe->next == NULL)
    {
        if (pipe->negation == 1)
            //pas sur ici
            return !exec_cmd(pipe->cmd);
        return exec_cmd(pipe->cmd);
    }
    else
    {
        if (pipe->negation == 1)
            return !exec_pipeline_next(pipe->next);
        return exec_pipeline_next(pipe->next);
    }
}

int exec_and_or_next(struct pipeline *pipe, struct and_or_next *andornext)
{
    if (andornext->next == NULL)
    {
        if (andornext->op == AND)
            return (exec_pipeline(pipe) && exec_pipeline(andornext->pipeline));
        else
            return (exec_pipeline(pipe) || exec_pipeline(andornext->pipeline));
    }
    else
    {
        //pas sur ici
        if (andornext->op == AND)
            return  (exec_pipeline(pipe) && exec_and_or_next(andornext->pipeline), andornext->next);
        else
            return  (exec_pipeline(pipe) || exec_and_or_next(andornext->pipeline), andornext->next);
    }
}

int exec_and_or(struct and_or *andor)
{
    if (andor->next == NULL)
        return exec_pipeline;
    else
        return exec_and_or_next(andor->pipeline, andor->next);
}

int exec_list_next(struct list_next *list_next)
{
    if (list_next == NULL)
        return 0;
    int k = exec_and_or(list_next->a_o); //return ???
    if (list_next != NULL)
        exec_list_next(list_next->next);
    return k;
}


int exec_list(struct list *list)
{
    if (list == NULL)
        return 0;
    exec_and_or(list->a_o); // return????
    if (list_next != NULL)
        int k = exec_list_next(list->next);
    return 0;
    //gerer sep
}


int exec(char *buffer)
{
    struct list *begin = build_list(buffer);
    if (begin == NULL)
        return 0;
    return exec_list(begin);
}



