#pragma once

#include "../struct/exec_struct.h"

int simple_redir(char *io, char *file, struct redir *redir, char *flag);

void reinit_redir(struct redir *redir);
