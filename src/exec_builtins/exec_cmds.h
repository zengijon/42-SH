#pragma once

#include "../struct/exec_struct.h"

//char *builtins[] = {"echo", "cd", "export", "continue", "break", "pitfall", "dot", "unset"};
/// Search les builtins et les executes sinon renvoit 127
int exec_builtins(char *cmd, char **parameters, struct exec_struct *e_x);
/// Cherche la fonction dans la liste de fonction en haut du .c
/// et l'execute avec micro shell
/// sinon retourne exec_builtins
int exec_cmds(char *cmd, char **parameters, struct exec_struct *e_x);

//int exec_function(int indic, char **parameters, int nb_param, struct exec_struct *e_x);
//int exec_function(int indic, struct exec_struct *e_x);


