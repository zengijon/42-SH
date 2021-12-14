//
// Created by jennie on 24/11/2021.
//
#include "mypipe.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int my_pipe(struct command *cmd, struct pipeline_next *next,
            struct exec_struct *e_x) /// mis des const
{
    pid_t child_pid;
    pid_t child_pid2;

    int pipefd[2];
    pipe(pipefd);

    child_pid = fork();

    if (child_pid == -1)
        errx(2, "fork failed");

    if (child_pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[1]); // Close write pipe du child
        close(pipefd[0]); // Close read pipe du child

        /// execute la commande de left_side
        exec_command(cmd, e_x);
        exit(0);
        ///
    }
    else
    {
        int wstatus;
        int val = waitpid(child_pid, &wstatus, 0);
        if (val == -1)
            errx(2, "Waitpid failed");

        child_pid2 = fork();

        if (child_pid2 == -1)
            errx(2, "fork 2 failed");

        if (child_pid2 == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // Close read pipe du parent
            close(pipefd[1]);

            /// execute la commande de right_side
            exec_pipeline_next(next, e_x);
            exit(0);
            ///
        }
        else
        {
            close(pipefd[1]);
            int wstatus2;
            int val2 = waitpid(child_pid2, &wstatus2, 0);

            if (val2 == -1)
                errx(2, "Waitpid 2 failed");

            close(pipefd[0]);
        }
    }
    return 0;
}
