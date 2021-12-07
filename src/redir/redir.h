#pragma once

#include "../struct/exec_struct.h"

int simple_redir(char *io, char *file, struct redir *redir, char *flag);

void reinit_redir(struct redir *redir);

int esp_redir(char *io, char *fd_esp, struct redir *redir, int flag);

int append_redir(char *io, char *file, struct redir *redir);
