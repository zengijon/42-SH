#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char **argv)
{
    if (argc < 3)
        errx(2, "need at least 2 parameters");
    char *buffer = malloc(sizeof(char) * 200);
    size_t buffercap = 200;
    size_t bufferlen = 0;
    for(int i = 2; i < argc; ++i)
    {
        bufferlen += strlen(argv[i]);
        if (buffercap < bufferlen)
        {
            buffer = realloc(buffer, buffercap + 200);
            buffercap += 200;
        }
        buffer = strcat(buffer, argv[i]);
        buffer = strcat(buffer, " ");
        bufferlen ++;
    }
    buffer[strlen(buffer) - 1] = '\0';
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0666);
    int a = dup(fileno(stdout));
    dup2(fd, 1);
    close(fd);
    int pid = fork();
    if (pid == 0)
    {
        execlp("/bin/sh", "supershell", "-c", buffer, NULL);

    }
    int wstatus;
    int child_pid = waitpid(pid, &wstatus, 0);
    if (child_pid == -1)
        errx(127, "fork error");
    dup2(a, 1);
    printf("%s exited with %d\n", argv[3], WEXITSTATUS(wstatus));
    close(a);
    return 0;
}
