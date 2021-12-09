#pragma once

#include "../struct/exec_struct.h"

//char *builtins[] = {"echo", "cd", "export", "continue", "break", "pitfall", "dot", "unset"};
int exec_builtins(char *cmd, int nb_params, char **parameters, struct exec_struct *e_x);
int exec_cmds(char *cmd, int nb_params, char **parameters, struct exec_struct *e_x);

//int exec_function(int indic, char **parameters, int nb_param, struct exec_struct *e_x);
int exec_function(int indic, struct exec_struct *e_x);


