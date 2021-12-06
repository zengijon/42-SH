#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "redir.h"

int simple_redir(char *io, char *file, struct redir *redir, char *flag)
{
    int io_nb;
    if (io == NULL)
    {
        if (strcmp(flag, "w") == 0)
            io_nb = 1;
        else
            io_nb = 0;
    }
    else
        io_nb = atoi(io);
    if (io_nb > 1024)
        return 1;
    FILE *new_file = fopen(file, flag);
    if (!new_file)
        return 1;
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


int append_redir(char *io, char *file, struct redir *redir)
{
    int io_nb;
    if (io[0] == '\0')
        io_nb = 1;
    else
        io_nb = atoi(io);
    if (io_nb > 1024)
        return 1;
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND);
    int new_fd = dup(io_nb);
    close(io_nb);
    dup2(fd, io_nb);
    close(fd);
    redir->new_fd = new_fd;
    redir->old_fd = io_nb;
    redir->file = NULL;
    return 0;
}

//
//int main(void)
//{
//    struct redir *n = malloc(sizeof(struct redir));
//    simple_redir("1", "test", n);
//    printf("in test\n");
//    reinit_redir(n);
//    printf("in stdout\n");
//    return 1;
//}
//
//int main(void)
//{
//    struct redir *n = malloc(sizeof(struct redir));
//    simple_redir("1", "test", n);
//    printf("in test\n");
//    reinit_redir(n);
//    printf("in stdout\n");
//    return 1;
//}

