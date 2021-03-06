//
// Created by jennie on 07/12/2021.
//

#ifndef INC_42SHEEEE_BUILTINS_H
#define INC_42SHEEEE_BUILTINS_H

#include "../../struct/exec_struct.h"
#include "../../exec/exec.h"

int my_echo(char *cmd);
char *concat_all(char **argv);
int my_cd(char **argv, struct exec_struct *e_x);
int my_unset(char **params, struct exec_struct *e_x);
int my_exit(char **parameters);
int my_export(char **params, struct exec_struct *e_x);
int my_pwd(struct exec_struct *e_x);
int generate_alias(struct exec_struct *e_x, char **parameters);
#endif // INC_42SHEEEE_BUILTINS_H
