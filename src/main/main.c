#include <stdio.h>
#include <stddef.h>
#include <err.h>

#include "../utils/file2buf.h"
int main(int argc, char **argv)
{
    if (argc != 2)
        errx(1, "need 1 parameter");
    char *buffer = file2buf(argv[1]);
    printf("%s\n", buffer);
    return 0;
}
