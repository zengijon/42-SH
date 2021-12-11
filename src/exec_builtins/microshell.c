#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int microshell(char *cmd, char **argv)
{
    printf("--%s\n", cmd);
    int pid = fork();
    if (pid == -1)
        errx(1, "fork failed");
    if (pid == 0)
        exit(execvp(cmd, argv));

    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(1, "waitpid error");
    return wstatus;
}
