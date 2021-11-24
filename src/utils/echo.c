#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <fnmatch.h>

void my_echo(char *cmd)
{
    int first = 0;
    int n_flag = 0;
    const char *separators = " \\\n\t";
    if (fnmatch("* -e *", cmd, 0) == 0)
        separators = " ";
    char *token = strtok(cmd, separators);
    if (strcmp(token, "echo") != 0)
        errx(1, "not an echo command");
    token = strtok(NULL, separators);
    while (token != NULL)
    {
        if (strcmp(token, "-n") == 0)
            n_flag = 1;
        else if (first == 0)
        {
            printf("%s", token);
            first++;
        }
        else
            printf(" %s", token);
        token = strtok(NULL, separators);
    }
    if (n_flag == 0)
        printf("\n");
}

//int main(void)
//{
//    char * cmd = malloc(40);
//    cmd = strcpy(cmd, "echo -n ceci est un test merci");
//    my_echo(cmd);
//    return 0;
//}
//
