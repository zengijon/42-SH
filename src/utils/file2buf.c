#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/hmalloc.h"

//struct free_list *list_malloc = NULL;

char *file2buf(char *filename)
{
    size_t len;
    FILE *fd = fopen(filename , "r");
    if (!fd)
        errx(1, "file doesnt exist");
    char *buffer = hcalloc(101 ,sizeof(char));
    size_t le = 0;
    while((len = fread(buffer + le, 1, 100, fd)) && (int) len != 0)
    {
        le += len;
        buffer = hrealloc(buffer, le + len);
    }
    buffer[le] = '\0';
    fclose(fd);
    return buffer;
}

char *expand_special_var(char *buffer, char *val_list)
{ //unacurate buffer size
    char *res = hcalloc(strlen(buffer) + strlen(val_list) * 256, 1);
    int single = 0;
    int double_ = 0;
    for (int i = 0; buffer[i] != 0; i++)
    {
        if (buffer[i] == '"')
            double_ = !double_;
        if (double_ == 0 && buffer[i] == '\'')
            single = !single;
        if (single == 0 && buffer[i] == '$' && buffer[i + 1] == '@')
        {
            strncpy(res, buffer, i);
            return expand_special_var(strcat(res, val_list), val_list);
        }
    }
    return res[0] == 0 ? buffer : res;
}
