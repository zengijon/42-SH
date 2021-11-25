//
// Created by jennie on 24/11/2021.
//
#include <err.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

///
#include <stdio.h>

int exec_pipe(const char **argv_left, const char **argv_right) /// mis des const
{
    pid_t child_pid;
    pid_t child_pid2;

    int pipefd[2];
    pipe(pipefd);

    child_pid = fork();

    if (child_pid == -1)
        errx(1, "fork failed");

    if (child_pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO); // sens ?

        close(pipefd[1]); // Close write pipe du child
        close(pipefd[0]); // Close read pipe du child

        /// execute la commande de left_side
        if (execvp(argv_left[0], (char * const*) argv_left) == -1)
            errx(1, "execvp failed");
        ///

        exit(EXIT_SUCCESS);
    }
    else
    {
        int wstatus;
        int val = waitpid(child_pid, &wstatus, 0);
        if (val == -1)
            errx(1, "Waitpid failed");


        child_pid2 = fork();

        if (child_pid2 == -1)
            errx(1, "fork 2 failed");

        if (child_pid2 == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // Close read pipe du parent
            close(pipefd[1]);

            /// execute la commande de right_side
            if(execvp(argv_right[0], (char * const*) argv_right) == -1)
                errx(1, "execvp 2 failed");
            ///

            exit(EXIT_SUCCESS);
        }
        else
        {
            close(pipefd[1]);
            int wstatus2;
            int val2 = waitpid(child_pid2, &wstatus2, 0);

            if (val2 == -1)
                errx(1, "Waitpid 2 failed");

            close(pipefd[0]); // Re close ?
            exit(EXIT_SUCCESS);
        }
    }
}

int main(void)
{
    const char *argv_left[3] = {"echo", "Hallo", NULL};
    const char *argv_right[4] = {"tr", "a", "d", NULL};
    printf("%d\n", exec_pipe(argv_left, argv_right));
    return 0;
}