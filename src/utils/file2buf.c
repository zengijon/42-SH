#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../memory/hmalloc.h"

// struct free_list *list_malloc = NULL;

char *file2buf(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "missing argument for source\n");
        return NULL;
    }
    size_t len;
    FILE *fd = fopen(filename, "r");
    if (!fd)
        errx(2, "file doesnt exist");
    char *buffer = hcalloc(101, sizeof(char));
    size_t le = 0;
    while ((len = fread(buffer + le, 1, 100, fd)) && (int)len != 0)
    {
        le += len;
        buffer = hrealloc(buffer, le + len);
    }
    buffer[le] = '\0';
    fclose(fd);
    return buffer;
}

char *expand_special_var(char *buffer, char *val_list)
{ // unacurate buffer size
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
            strcat(res, val_list);
            return strcat(res, expand_special_var(buffer + i + 2, val_list));
        }
    }
    return res[0] == 0 ? buffer : res;
}

char *stdin2buf(void)
{
    size_t len;
    char *buffer = hcalloc(101, sizeof(char));
    size_t le = 0;
    while ((len = read(0, buffer + le, 100)) && (int)len != 0)
    {
        le += len;
        buffer = hrealloc(buffer, le + len);
    }
    buffer[le] = '\0';
    return buffer;
}
