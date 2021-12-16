//
// Created by clousty8 on 09/12/2021.
//
#pragma once 

#include "../struct/exec_struct.h"

char *build_shell_buffer(char *subshell);

char *build_shell_buffer2(char *subshell);
//int exec_subshell4(char *buffer, struct exec_struct *e_x, char **buf);

int exec_subshell(char *buffer, struct exec_struct *e_x, char **buf);
int exec_subshell2(char *buffer, struct exec_struct *e_x, char **buf);

int exec_subshell3(char *buffer, struct exec_struct *e_x, char **buf);