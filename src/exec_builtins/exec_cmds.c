#include "exec_cmds.h"

#include <string.h>

#include "../memory/hmalloc.h"
#include "builtins/builtins.h"
#include "microshell.h"
#include "../utils/usefull_fonction.h"

char *all_cmd[] = {"ls", "pwd", "exit", "cat"};

int indice_search_name(char *name, struct exec_struct *e_x)
{
    for (int i = 0; i < e_x->f_l_len; ++i)
        if (strcmp(name, e_x->f_l[i].name) == 0)
            return i;
    return -1;
}

int exec_function(int indic, char **parameters,struct exec_struct *e_x)
{
    struct var_list *save = hcalloc(e_x->v_l_size, sizeof(struct var_list));
    memcpy(save, e_x->v_l, e_x->v_l_size * sizeof(struct var_list));
    char *empty = hcalloc(1,1);;
    int nb = atoi(get_value_in_vl( e_x, "#"));
    int nb_param = 0;
    for (int i = 0; parameters[i] != 0; ++i)
        nb_param++;
    for (int i = 0; i <= nb; ++i)
            e_x->v_l[i].value = empty;
    for (int i = 0; i < nb_param; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), parameters[i], e_x);
    assign_var("#", my_itoa(nb_param - 1, hcalloc(10,1)), e_x);
    int res = exec_command(e_x->f_l[indic].cmd, e_x);
    for (int i = 0; i < nb_param; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), empty, e_x);
    for (int i = 0; i <= nb; ++i)
        assign_var(my_itoa(i, hcalloc(1, 8)), save[i].value, e_x);
    assign_var("#", my_itoa(nb, hcalloc(10,1)), e_x);
    return res;
}

int exec_builtins(char *cmd, char **parameters, struct exec_struct *e_x)
{
    int indic = 0;
    char *buffer = hcalloc(strlen(cmd) + 8192, sizeof(char));
    strcat(buffer, cmd);
    strcat(buffer, " ");
    for (int j = 1; parameters[j] != 0; ++j)
    {
        //buffer = hrealloc(buffer, strlen(parameters[j]) + strlen(buffer) + 2);
        strcat(buffer, parameters[j]);
        strcat(buffer, " ");
    }
    if (strcmp(cmd, "echo") == 0)
        return my_echo(buffer);
    else if (strcmp(cmd, "cd") == 0)
        return my_cd(parameters, e_x);
    else if (strcmp(cmd, "export") == 0)
        return 0;
    else if (strcmp(cmd, "continue") == 0)
        return 0;
    else if (strcmp(cmd, "break") == 0)
        return 0;
    else if (strcmp(cmd, "pitfall") == 0)
        return 0;
    else if (strcmp(cmd, "dot") == 0)
        return 0;
    else if (strcmp(cmd, "unset") == 0)
        return 0;
    else if (strcmp(cmd, "true") == 0)
        return 0;
    else if (strcmp(cmd, "false") == 0)
        return 1;
    else if ((indic = indice_search_name(cmd, e_x)) != -1)
        //return exec_function(indic, parameters, nb_params, e_x);
        return exec_function(indic, parameters, e_x);
    else
        return 127;
}

int exec_cmds(char *cmd, char **parameters, struct exec_struct *e_x)
{
    int nb_cmd = 4;
    if (strlen(cmd) == 0)
        return 127;
    for (int i = 0; i < nb_cmd; ++i)
    {
        if (strcmp(cmd, all_cmd[i]) == 0)
            return microshell(cmd, parameters);
    }
    return exec_builtins(cmd, parameters, e_x);
}

//int exec_function(int indic, char **parameters, int nb_param, struct exec_struct *e_x)
//{
//    return exec_command(e_x->f_l[indic].cmd, e_x);
//}

//struct exec_struct *exec_struct_copy_fill(struct exec_struct *e_x)
//{
//    struct exec_struct *res = copy_special_var(e_x);
//    /// il faut copier coller la struct e_x puis remplacer uniauement les variables speciales pour les faires matcherts avec les parametres de la fonction
//    /// et stocker les anciennes valeurs des variables speciales du pgm globale
//    /// apres l'execution de exec_command, il faut remettre les anciennes valeurs prealablements stokee dans exec_struct
//    return NULL;
//}
//
//struct exec_struct *copy_special_var(struct exec_struct *e_x)
//{
//    struct exec_struct *res = hcalloc(1, sizeof(struct exec_struct));
//
//}
//int main(void)
//{
//    char *p[] = {"-n", "bonjour", "bite"};
//    return exec_cmds("echo", 3, p);
//    return 0;
//}
