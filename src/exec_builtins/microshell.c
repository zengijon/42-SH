#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

//int microshell(char *cmd)
//{
//    int pid = fork();
//    if (pid == 0)
//        execlp("/bin/sh", "supershell", "-c", cmd, NULL);
////    execvp("find", argv);
//    int wstatus;
//    int child_pid = waitpid(pid, &wstatus, 0);
//    if (child_pid == -1)
//        errx(1, "fork error");
//    return 0;
//}
//
int microshell(char *cmd, char **argv)
{
    int pid = fork();
    if (pid == -1)
        errx(1, "fork failed");
    if (pid == 0)
    {
        if (strcmp(cmd, "cat") == 0)
        {
            return execlp("/bin/sh", "blandsfkjh", "-c", "cat -e", NULL);
        }
        return execvp(cmd, argv);
    }

    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(1, "waitpid error");
    return wstatus;
}
