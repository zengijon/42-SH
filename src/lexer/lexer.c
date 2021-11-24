#include "lexer.h"

#include <err.h>
#include <string.h>

int find_c(char c, char *buffer)
{
    int i = 0;
    for (; buffer[i] != 0 && buffer[i] != c; ++i)
    {
        if (buffer[i] == '\\')
            ++i;
    }
    return i;
}

int skip_space(char *buffer)
{
    int i = 0;
    while (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n')
        ++i;
    return i;
}

int get_end(char *buffer)
{
    int res = skip_space(buffer);
    while (buffer[res] != 0 && buffer[res] != ';' && buffer[res] != '\n') // and && and ||
    {
        if (buffer[res] == '\'' || buffer[res] == '"')
        {
            res += find_c(buffer[res], buffer + res + 1);

            //A vérifier faudra exit
            if (buffer[res] == 0)
                errx(1, "Guillemet pas fermé");
        }
        if (buffer[res] == '\\')
            res++;
        res++;
    }
    return res;
}

//Trop simplifié
int isvar(char *buffer, int len, int i)
{
    for (; i < len; ++i)
    {
        if (buffer[i] == '\\')
            ++i;
        if (buffer[i] == '=')
            return 1;
    }
    return 0;
}

enum lex get_type(char *buffer, int len)
{
    int i = skip_space(buffer);
    if (isvar(buffer, len, i))
        return VAR;
    if (strncmp(buffer + i, "if ", 3))
        return IF;
    if (strncmp(buffer + i, "&& ", 3))
        return AND;
    if (strncmp(buffer + i, "|| ", 3))
        return OR;
    if (strncmp(buffer + i, "else ", 3) && strncmp(buffer + i, "then ", 3)
        && strncmp(buffer + i, "elif ", 3) && strncmp(buffer + i, "do ", 3)
        && strncmp(buffer + i, "done ", 3))
        return TOKEN;
    return CMD;
}


void check_then(char **buffer)
{
    if (strncmp(*buffer + i, "then ", 5))
        errx(1, "missing then");
    buffer += 5;
}


void check_fi(char **buffer)
{
    if (strncmp(*buffer + i, "fi ", 3))
        errx(1, "missing then");
    buffer += 3;
}

void check_else(char **buffer)
{
    if (strncmp(*buffer + i, "elif ", 5))
    {
        buffer[1] = ' ';
        buffer[2] = ' ';
        buffer + 2;
        return;
    }
    if (strncmp(*buffer + i, "else ", 5))
        ;
    buffer += 5;
}

void build_if(char **buffer, struct fifo_lex *node)
{
    node->content.condition = build_node(buffer, 1);
    check_then(buffer);
    node->l_true = build_node(buffer, 1);
    if (check_else(buffer))
        node->r_false = build_node(buffer, 1);
    check_fi(buffer);
}

void build_cdm(char **buffer, struct fifo_lex *node, int rec)
{
    int end = get_end(buffer);
    node->content.data = hcalloc(sizeof(char *), end + 1);
    strncpy(node->content.data, buffer, end);
    buffer += end;
    struct fifo_lex *next= NULL;
    if (rec == NULL)
        next = build_node(buffer, rec);
    node->l_true = next;
    node->r_false = next;
}


void build_or(char **buffer, struct fifo_lex *node, int rec)
{
    int end = get_end(buffer);
    node->content.data = hcalloc(sizeof(char *), end + 1);
    strncpy(node->content.data, buffer, end);
    buffer += end;
    struct fifo_lex *next= NULL;
    if (rec == NULL)
        next = build_node(buffer, rec);
    node->l_true = next;
    node->r_false = next;
}


struct fifo_lex *build_node(char **buffer, int rec)
{
    struct fifo_lex *node = hcalloc(sizeof(struct fifo_lex), 1);
    node->type = get_type(*buffer, get_len(*buffer));
    switch (node->type)
    {
    case IF:
        build_if(buffer, node);
    case AND:
        build_and(buffer, node);
    case OR:
        build_or(buffer, node);
    case VAR:
        build_var(buffer, node, rec);
    case CMD:
        build_cdm(buffer, node);
    case TOKEN:
        return NULL;
    }
    return node;
}
