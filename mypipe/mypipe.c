//
// Created by jennie on 24/11/2021.
//
#include <err.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

///
#include <stdio.h>

int exec_pipe(char **argv_left, char **argv_right)
{
    pid_t child_pid;
    pid_t child_pid2;

    char *buf;

    int pipefd[2];
    pipe(pipefd);

    child_pid = fork();

    if (child_pid == -1)
        return 0;

    if (child_pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO); // sens ?

        close(pipefd[1]); // Close write pipe du child
        close(pipefd[0]); // Close read pipe du child

        if (execvp(argv_left[0], argv_left) == -1)
            return 0;

        exit(EXIT_SUCCESS);
    }
    else
    {
        int wstatus;
        int val = waitpid(child_pid, &wstatus, 0);

        child_pid2 = fork();

        if (child_pid2 == -1)
            return 0;

        if (child_pid2 == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // Close read pipe du parent
            close(pipefd[1]);

            if(execvp(argv_right[0], argv_right)) == -1)
                return 0;
            exit(EXIT_SUCCESS);
        }

        int wstatus;
        int val = waitpid(child_pid2, &wstatus, 0);

        while((read(pipefd[0], buf, 0)) > 0)
            printf("%s", buf);

        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
}

int main(void)
{
    const char *argv_left[3] = {"echo", "Hallo", NULL};
    const char *argv_right[4] = {"tr", "a", "e", NULL};
    printf("%d\n", exec_pipe(argv_left, argv_right));
    return 0;

}