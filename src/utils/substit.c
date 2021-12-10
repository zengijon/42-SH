//
// Created by clousty8 on 09/12/2021.
//

#include "substit.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../memory/hmalloc.h"
#include "../struct/exec_struct.h"
#include "../exec/variable_expention.h"

int exec_42sh(char *buffer, int pretty_print, struct exec_struct *e_x);

char *build_shell_buffer(char *subshell)
{
    int i = 0;
    int nb_parenthese = 1;

    char *exec_buffer = hcalloc(strlen(subshell) + 20, 1);
    exec_buffer = strcat(exec_buffer, "(");
    while (subshell[i] != '\0' && nb_parenthese != 0)
    {
        if (subshell[i] == '(')
            ++nb_parenthese;
        if (subshell[i] == ')')
            --nb_parenthese;
        char *tmp = hcalloc(2,1);
        tmp[0] = subshell[i];
        exec_buffer = strcat(exec_buffer, tmp);
        free(tmp);
        ++i;
    }
    if (subshell[i] == '\0' && nb_parenthese != 0)
        return NULL;
    exec_buffer = strcat(exec_buffer, ">fabbec_42");
    return exec_buffer;
}

int exec_subshell(char *buffer, struct exec_struct *e_x, char **buf)
{
    int len;
    char *subshell_buff = build_shell_buffer(buffer);
    int k = strlen(subshell_buff) - 11;
    int res = exec_42sh(subshell_buff, 1, e_x);//include ??
    FILE *file = fopen("fabbec_42", "r");
    int le = 0;
    *buf = hcalloc(100 + 8192, sizeof(char));
    while ((len = read(fileno(file), *buf + le, 100)) && len != 0)
    {
        if ( len == -1)
            errx(2, "read error");
        le += len;
        //*buf = hrealloc(*buf, le * 2 + 1);
    }
    char *tmp = hcalloc(strlen(*buf) + 8192, sizeof(char));
    strncpy(tmp,*buf, strlen(*buf) - 1);
    *buf = strcat(tmp, search_for_dollar(buffer + k, e_x));
    return res;
}

//int main(void)
//{
//    printf("%s\n", build_shell_buffer("bite(ls))bonjour"));
//}
