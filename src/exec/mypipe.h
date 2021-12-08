#ifndef MYPIPE_H
#define MYPIPE_H

#include "exec.h"

int my_pipe(struct command *cmd, struct pipeline_next *next,
            struct exec_struct *e_x);

#endif /* ! MYPIPE_H */
