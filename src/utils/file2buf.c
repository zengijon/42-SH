#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <err.h>

#include "../memory/hmalloc.h"

char *file2buf(char *filename)
{
    size_t len;
    FILE *fd = fopen(filename , "r");
    if (!fd)
        errx(1, "file doesnt exist");
    char *buffer = hcalloc(101 ,sizeof(char));
    size_t le = 0;
    while((len = fread(buffer + le, 1, 100, fd)) && len != 0)
    {
        le += len;
        buffer = hrealloc(buffer, le + len);
    }
    buffer[le] = '\0';
    return buffer;
}
