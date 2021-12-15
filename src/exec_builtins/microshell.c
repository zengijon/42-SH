#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int microshell(char *cmd, char **argv)
{
    int pid = fork();
    if (pid == -1)
        errx(2, "fork failed");
    if (pid == 0)
    {
        int res = execvp(cmd, argv);
        exit(res == -1 ? 127 : res);
    }

    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(2, "waitpid error");
    return WEXITSTATUS(wstatus);
}
