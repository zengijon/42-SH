#pragma once

struct redir
{
    int new_fd;
    int old_fd;
    FILE *file;
};
