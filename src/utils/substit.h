//
// Created by clousty8 on 09/12/2021.
//

#ifndef INC_42SHEEEE_SUBSTIT_H
#define INC_42SHEEEE_SUBSTIT_H

#include "../struct/exec_struct.h"

char *build_shell_buffer(char *subshell);

int exec_subshell(char *buffer, struct exec_struct *e_x, char **buf);

#endif // INC_42SHEEEE_SUBSTIT_H
