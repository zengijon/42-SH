#include <err.h>
#include <getopt.h>
#include <stddef.h>
#include <string.h>

#include "../exec/exec.h"
#include "../memory/hmalloc.h"
#include "../parser/parser.h"
#include "../test/test_parser/print_parser.h"
#include "../utils/file2buf.h"
#include "stdio.h"

struct free_list *list_malloc = NULL;

int exec_42sh(char *buffer, int pretty_print)
{
    int res = 0;
    struct list *list;
    struct lexer *lex = lexer_new(buffer);
    struct exec_struct *e_x = hcalloc(1, sizeof(struct exec_struct));
    while ((list = build_list(lex)) != NULL)
    {
        if (pretty_print == 1)
            print_list(list);
        res = exec_list(list, e_x);
    }
    return res;
}

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
    int pretty_print = 0;

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
            pretty_print = 1;
            break;
        default:
            errx(1, "bad option");
        }
    }
    if (argc <= optind)
        errx(1, "missing argv"); // errx(1, "missing parameter"); //handel
                                 // reading stdin

    buffer = file2buf(argv[optind]);
    exec_42sh(buffer, pretty_print);
    return 0;
}
