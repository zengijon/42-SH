#include "exec_cmds.h"

#include <string.h>

#include "../memory/hmalloc.h"
#include "echo.h"
#include "microshell.h"

char *all_cmd[] = { "ls", "pwd", "exit" };

int exec_builtins(char *cmd, int nb_params, char **parameters)
{
    char *buffer = hmalloc(strlen(cmd) + 1);
    strcat(buffer, cmd);
    strcat(buffer, " ");
    for (int j = 0; j < nb_params; ++j)
    {
        buffer = hrealloc(buffer, strlen(parameters[j]) + strlen(buffer) + 1);
        strcat(buffer, parameters[j]);
        strcat(buffer, " ");
    }
    if (strcmp(cmd, "echo") == 0)
        return my_echo(buffer);
    else if (strcmp(cmd, "cd") == 0)
        return 0;
    else if (strcmp(cmd, "export") == 0)
        return 0;
    else if (strcmp(cmd, "continue") == 0)
        return 0;
    else if (strcmp(cmd, "break") == 0)
        return 0;
    else if (strcmp(cmd, "pitfall") == 0)
        return 0;
    else if (strcmp(cmd, "dot") == 0)
        return 0;
    else if (strcmp(cmd, "unset") == 0)
        return 0;
    else if (strcmp(cmd, "true") == 0)
        return 0;
    else if (strcmp(cmd, "false") == 0)
        return 1;
    else
        return 127;
}

int exec_cmds(char *cmd, int nb_params, char **parameters)
{
    int nb_cmd = 3;
    for (int i = 0; i < nb_cmd; ++i)
    {
        char *buffer = hmalloc(strlen(cmd) + 1);
        strcat(buffer, cmd);
        strcat(buffer, " ");
        if (strcmp(cmd, all_cmd[i]) == 0)
        {
            for (int j = 0; j < nb_params; ++j)
            {
                buffer = hrealloc(buffer,
                                  strlen(parameters[j]) + strlen(buffer) + 1);
                strcat(buffer, parameters[j]);
                strcat(buffer, " ");
            }
            return microshell(buffer);
        }
    }
    return exec_builtins(cmd, nb_params, parameters);
}
//
//int main(void)
//{
//    char *p[] = {"-n", "bonjour", "bite"};
//    return exec_cmds("echo", 3, p);
//    return 0;
//}
