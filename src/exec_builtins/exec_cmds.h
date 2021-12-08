#pragma once

// char *builtins[] = {"echo", "cd", "export", "continue", "break", "pitfall",
// "dot", "unset"};
int exec_builtins(char *cmd, int nb_params, char **parameters);
int exec_cmds(char *cmd, int nb_params, char **parameters);
