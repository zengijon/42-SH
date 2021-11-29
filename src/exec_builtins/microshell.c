#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(1, "need only one parameter");

    int pid = fork();
    if (pid == 0)
    {
        execlp("/bin/sh", "supershell", "-c", argv[1]);
    }

    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(1, "fork error");
    printf("process exit status: %d\n", WEXITSTATUS(wstatus));
    return 0;
}
