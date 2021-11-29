#include <err.h>
#include <getopt.h>
#include <stddef.h>
#include <string.h>
#include "stdio.h"

#include "../exec/exec.h"
#include "../memory/hmalloc.h"
#include "../parser/parser.h"
#include "../test/test_parser/print_parser.h"
#include "../utils/file2buf.h"

struct free_list *list_malloc = NULL;

int main(int argc, char **argv)
{
    static const struct option longOpts[] = {
        { "pretty_print", no_argument, NULL, 'p' },
        { "cmd", required_argument, NULL, 'c' },
        { "verbose", no_argument, NULL, 'v' },
        { NULL, 0, NULL, 0 }
    };
    static const char *optString = "pc:v";
    int index;
    int opt;
    char *buffer;
    while ((opt = getopt_long(argc, argv, optString, longOpts, &index))
           && opt != -1)
    {
        switch (opt)
        {
        case 'c':
            buffer = hmalloc(sizeof(char) * strlen(optarg));
            strcpy(buffer, optarg);
            break;
        case 'v':
            break;
        case 'p':
            break;
        default:
            errx(1, "bad option");
        }
    }
    if (optind == 1)
        buffer = file2buf(argv[1]);
    struct list *list;
    struct lexer *lex = lexer_new(buffer);
    while ((list = build_list(lex)) != NULL)
    {
        //print_list(list);
        exec_list(list);
    }
    return 0;
}
