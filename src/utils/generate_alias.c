#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../memory/hmalloc.h"

struct free_list *list_malloc = NULL;

char *generate_alias(char *input, char *var, char *val)
{
    int max_len = strlen(input) * strlen(var);
    char *buffer = hcalloc(strlen(input) * strlen(var), sizeof(char));
    int len = strlen(var);
    for (int i = 0 ; input[i] != '\0'; ++i)
    {
        if (i == max_len - 1)
        {
            buffer = hrealloc(buffer, max_len * 2);
            max_len *= 2;
        }
        if (strncmp(&input[i], var, len) == 0)
        {
            buffer = strcat(buffer, val);
            i += len - 1;
        }
        else
        {
            char *tmp = calloc(2,1);
            tmp[0] = input[i];
            buffer = strcat(buffer, tmp);
            free(tmp);
        }
    }
    return buffer;
}

int main(void)
{
    printf("%s\n", generate_alias("lucas aime la queue et il aime la cadette aime", "aime", "adore"));
    return 0;
}
