#include "exec_cmds.h"

#include <err.h>
#include <string.h>

#include "../memory/hmalloc.h"
#include "../utils/file2buf.h"
#include "../utils/usefull_fonction.h"
#include "builtins/builtins.h"
#include "microshell.h"

char *all_cmd[] = { "ls", "pwd", "cat" };

int indic_search_name(char *name, struct exec_struct *e_x)
{
    for (int i = 0; i < e_x->f_l_len; ++i)
        if (strcmp(name, e_x->f_l[i].name) == 0)
            return i;
    return -1;
}
struct exec_struct *fill_copy_var_list(struct exec_struct *e_x, int nb,
                                       int nb_param, char **parameters)
{
    char *empty = hcalloc(1, 1);
    for (int i = 0; i <= nb; ++i)
        e_x->v_l[i].value = empty;
    for (int i = 0; i < nb_param; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), parameters[i], e_x);
    assign_var("#", my_itoa(nb_param - 1, hcalloc(10, 1)), e_x);
    return e_x;
}

struct exec_struct *restore_var_list(struct exec_struct *e_x, int nb,
                                     int nb_param, struct var_list *save)
{
    char *empty = hcalloc(1, 1);
    for (int i = 0; i < nb_param; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), empty, e_x);
    for (int i = 0; i <= nb; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), save[i].value, e_x);
    assign_var("#", my_itoa(nb, hcalloc(10, 1)), e_x);
    return e_x;
}

int exec_function(int indic, char **parameters, struct exec_struct *e_x)
{
    struct var_list *save = hcalloc(e_x->v_l_size, sizeof(struct var_list));
    memcpy(save, e_x->v_l, e_x->v_l_size * sizeof(struct var_list));
    // char *empty = hcalloc(1,1);
    int nb = atoi(get_value_in_vl(e_x, "#"));
    int nb_param = 0;
    for (int i = 0; parameters[i] != 0; ++i)
        nb_param++;
    e_x = fill_copy_var_list(e_x, nb, nb_param, parameters);
    //    for (int i = 0; i <= nb; ++i)
    //            e_x->v_l[i].value = empty;
    //    for (int i = 0; i < nb_param; ++i)
    //        assign_var(my_itoa(i, hcalloc(1, 8)), parameters[i], e_x);
    //    assign_var("#", my_itoa(nb_param - 1, hcalloc(10,1)), e_x);
    int res = exec_command(e_x->f_l[indic].cmd, e_x);
    e_x = restore_var_list(e_x, nb, nb_param, save);
    //    for (int i = 0; i < nb_param; ++i)
    //        assign_var(my_itoa(i, hcalloc(1, 8)), empty, e_x);
    //    for (int i = 0; i <= nb; ++i)
    //        assign_var(my_itoa(i, hcalloc(1, 8)), save[i].value, e_x);
    //    assign_var("#", my_itoa(nb, hcalloc(10,1)), e_x);
    return res;
}

int exec_42sh(char *buffer, int pretty_print, struct exec_struct *e_x);

int exec_builtins(char *cmd, char **parameters, struct exec_struct *e_x)
{
    int indic = 0;
    char *buffer = hcalloc(strlen(cmd) + 8192, sizeof(char));
    strcat(buffer, cmd);
    strcat(buffer, " ");
    for (int j = 1; parameters[j] != 0; ++j)
    {
        // buffer = hrealloc(buffer, strlen(parameters[j]) + strlen(buffer) +
        // 2);
        strcat(buffer, parameters[j]);
        strcat(buffer, " ");
    }
    if (strcmp(cmd, "echo") == 0)
        return my_echo(buffer);
    else if (strcmp(cmd, "exit") == 0)
        return my_exit(parameters);
    else if (strcmp(cmd, "cd") == 0)
        return my_cd(parameters, e_x);
    else if (strcmp(cmd, "export") == 0)
        return my_export(parameters, e_x);
    else if (strcmp(cmd, "continue") == 0)
    {
        if (e_x->loop_nb == 0)
            return 0;
        int val = (parameters[1] == NULL ? 1 : atoi(parameters[1])); // possible segfault when parameters[1] is NULL
        if (val <= 0)
            errx(2, "break parameters < 1");
        return 1000 - 1 + (val > e_x->loop_nb ? e_x->loop_nb : val);
    }
    else if (strcmp(cmd, "break") == 0)
    {
        if (e_x->loop_nb == 0)
            return 0;
        int val = parameters[1] == NULL ? 1 : atoi(parameters[1]); // possible segfault when parameters[1] is NULL
        if (val <= 0)
            errx(2, "break parameters < 1");
        return 1000000 - 1 + (val > e_x->loop_nb ? e_x->loop_nb : val);
    }
    else if (strcmp(cmd, ".") == 0)
        return exec_42sh(file2buf(parameters[1]), 0, e_x);
    else if (strcmp(cmd, "unset") == 0)
        return my_unset(parameters, e_x);
    else if (strcmp(cmd, "alias") == 0)
        return generate_alias(e_x, parameters);
    else if (strcmp(cmd, "true") == 0)
        return 0;
    else if (strcmp(cmd, "false") == 0)
        return 1;
    else if (strcmp(cmd, "pwd") == 0)
        return my_pwd(e_x);
    else if ((indic = indic_search_name(cmd, e_x)) != -1)
        return exec_function(indic, parameters, e_x);
    else
    {
        fprintf(stderr, "%s: command not found\n", cmd);
        return 127;
    } // pb si la fonction n'existe plus il faut le dire et pas
                    // renvoye 127 + finir avec 0
}

int exec_cmds(char *cmd, char **parameters, struct exec_struct *e_x)
{
    int res = 0;
    if ((res = exec_builtins(cmd, parameters, e_x)) == 127)
        return microshell(cmd, parameters);
    return res;
}

// int main(void)
//{
//     char *p[] = {"-n", "bonjour", "bite"};
//     return exec_cmds("echo", 3, p);
//     return 0;
// }
