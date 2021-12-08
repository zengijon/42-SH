#include "exec_cmds.h"

#include <string.h>

#include "../memory/hmalloc.h"
#include "builtins/builtins.h"
#include "microshell.h"

char *all_cmd[] = {"ls", "pwd", "exit"};

int indice_search_name(char *name, struct exec_struct *e_x)
{
    for (int i = 0; i < e_x->f_l_len; ++i)
        if (strcmp(name, e_x->f_l[i].name) == 0)
            return i;
    return -1;
}

int exec_builtins(char *cmd, int nb_params, char **parameters, struct exec_struct *e_x)
{
    int indic = 0;
    char *buffer = hcalloc(strlen(cmd) + 2, sizeof(char));
    strcat(buffer, cmd);
    strcat(buffer, " ");
    for (int j = 0; j < nb_params; ++j)
    {
        buffer = hrealloc(buffer, strlen(parameters[j]) + strlen(buffer) + 2);
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
        return exec_function(indic, e_x);
    else
        return 127;
}

int exec_cmds(char *cmd, int nb_params, char **parameters, struct exec_struct *e_x)
{
    int nb_cmd = 3;
    if (strlen(cmd) == 0)
        return 127;
    for (int i = 0; i < nb_cmd; ++i)
    {
        if (strcmp(cmd, all_cmd[i]) == 0)
            return microshell(cmd, parameters);
    }
    return exec_builtins(cmd, nb_params, parameters, e_x);
}

//int exec_function(int indic, char **parameters, int nb_param, struct exec_struct *e_x)
//{
//    return exec_command(e_x->f_l[indic].cmd, e_x);
//}
int exec_function(int indic, struct exec_struct *e_x)
{
    struct exec_struct *copy = exec_struct_copy_fill(e_x);
    int res = exec_command(e_x->f_l[indic].cmd, copy);
    return res;
}

struct exec_struct *exec_struct_copy_fill(struct exec_struct *e_x)
{
    /// il faut copier coller la struct e_x puis remplacer uniauement les variables speciales pour les faires matcherts avec les parametres de la fonction
    /// et stocker les anciennes valeurs des variables speciales du pgm globale
    /// apres l'execution de exec_command, il faut remettre les anciennes valeurs prealablements stokee dans exec_struct
    return NULL;
}
//int main(void)
//{
//    char *p[] = {"-n", "bonjour", "bite"};
//    return exec_cmds("echo", 3, p);
//    return 0;
//}
