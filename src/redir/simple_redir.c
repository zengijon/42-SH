#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "redir.h"

int simple_redir(char *io, char *file, struct redir *redir)
{
    int io_nb;
    if (io == NULL)
        io_nb = 1;
    else
        io_nb = atoi(io);
    if (io_nb > 1024)
        return 1;
    FILE *new_file = fopen(file, "w");
    int fd = fileno(new_file);
    int new_fd = dup(io_nb);
    close(io_nb);
    dup2(fd, io_nb);
    close(fd);
    redir->new_fd = new_fd;
    redir->old_fd = io_nb;
    redir->file = new_file;
    return 0;
}

void reinit_redir(struct redir *redir)
{
    fflush(NULL);
    close(redir->old_fd);
    dup2(redir->new_fd, redir->old_fd);
    close(redir->new_fd);
    fclose(redir->file);
}


int main(void)
{
    struct redir *n = malloc(sizeof(struct redir));
    simple_redir("1", "test", n);
    printf("in test\n");
    reinit_redir(n);
    printf("in stdout\n");
    return 1;
}
