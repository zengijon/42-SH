#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int microshell(char *cmd)
{
    int pid = fork();
    if (pid == 0)
        execlp("/bin/sh", "supershell", "-c", cmd, NULL);
    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(1, "fork error");
    printf("process exit status: %d\n", WEXITSTATUS(wstatus));
    return 0;
}
