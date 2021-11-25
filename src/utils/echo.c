#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <fnmatch.h>

char *replace_newline(char *token)
{
    char *tmp = calloc(strlen(token) * 2, sizeof(char));
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
            char *j = calloc(2, 1);
            j[0] = token[i];
            strcat(tmp, j);
        }
        ++i;
    }
    return tmp;
}

void my_echo(char *cmd)
{
    int first = 0;
    int n_flag = 0;
    int e_flag = 0;
    int word_flag = 0;
    const char *separators = " ";
    char *res = calloc(strlen(cmd) * 2, sizeof(char));
    char *token = strtok(cmd, separators);
    if (strcmp(token, "echo") != 0)
        errx(1, "not an echo command");
    token = strtok(NULL, separators);
    while (token != NULL)
    {
        if (strcmp(token, "-n") == 0 && word_flag == 0)
            n_flag = 1;
        else if (strcmp(token, "-e") == 0 && word_flag == 0)
            e_flag = 1;
        else if ((strcmp(token, "-ne") == 0 || strcmp(token, "-en") == 0) && word_flag == 0)
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
                char *tmp = replace_newline(token);
                strcat(res, tmp);
                free(tmp);
            }
            else
            {
                strcat(res, token);
            }
            first++;
        }

        token = strtok(NULL, separators);
    }
    printf("%s", res);
    fflush(stdout);
    free(res);
    if (n_flag == 0)
        printf("\n");
}

//int main(void)
//{
//    char * cmd = malloc(40);
//    cmd = strcpy(cmd, "echo cou\\ncou");
//    my_echo(cmd);
//    return 0;
//}
//
