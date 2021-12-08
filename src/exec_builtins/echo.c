#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/hmalloc.h"
#include "unistd.h"

char *replace_newline(char *token)
{
    char *tmp = hcalloc(strlen(token) * 2, sizeof(char));
    int i = 0;
    while (token[i] != '\0')
    {
        if (strncmp(&token[i], "\\n", 2) == 0)
        {
            strcat(tmp, "\n");
            ++i;
        }
        else if (strncmp(&token[i], "\\t", 2) == 0)
        {
            strcat(tmp, "\t");
            ++i;
        }
        else if (strncmp(&token[i], "\\", 1) == 0)
        {
            strcat(tmp, "\\");
            ++i;
        }
        else
        {
            char *j = hcalloc(2, 1);
            j[0] = token[i];
            strcat(tmp, j);
        }
        ++i;
    }
    return tmp;
}

char *create_token(char *buffer)
{
    int i = 0;
    char *tmp = hcalloc(strlen(buffer) + 1, sizeof(char));
    while (*buffer != '\0' && *buffer != ' ')
    {
        tmp[i++] = *buffer++;
    }
    return tmp;
}

int my_echo(char *cmd)
{
    int first = 0;
    int n_flag = 0;
    int e_flag = 0;
    int word_flag = 0;
    char *res = hcalloc(strlen(cmd) * 2, sizeof(char));
    if (strncmp(cmd, "echo ", 5) != 0)
    {
        write(2, "not an echo command", 20);
        return 127;
    }
    char *tmp1 = hcalloc(strlen(cmd), 1);
    tmp1 = strcpy(tmp1, &cmd[5]);
    char *tmp = tmp1;
    while (strlen(tmp) != 0)
    {
        char *token = create_token(tmp);
        tmp += strlen(token) + 1;
        if (strcmp(token, "-n") == 0 && word_flag == 0)
            n_flag = 1;
        else if (strcmp(token, "-e") == 0 && word_flag == 0)
            e_flag = 1;
        else if ((strcmp(token, "-ne") == 0 || strcmp(token, "-en") == 0)
                 && word_flag == 0)
        {
            e_flag = 1;
            n_flag = 1;
        }
        else
        {
            if (first != 0)
                strcat(res, " ");
            word_flag = 1;
            if (e_flag == 1)
            {
                char *tmp_ = replace_newline(token);
                strcat(res, tmp_);
            }
            else
            {
                strcat(res, token);
            }
            first++;
        }
    }
    printf("%s", res);
    fflush(stdout);
    if (n_flag == 0)
        printf("\n");
    return 0;
}

// int main(void)
//{
//    char * cmd = malloc(40);
//    cmd = strcpy(cmd, "echo coucou bnitgjker fkfkf");
//    my_echo(cmd);
//    return 0;
//}
