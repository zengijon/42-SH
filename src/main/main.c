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
    while ((list = build_list(lex)) != NULL)
    {
        if (pretty_print == 1)
            print_list(list);
        res = exec_list(list);
    }
    return res;
}

int main(int argc, char **argv)
{
  if(argc == 1)
      return 0;
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
    int c = 0;

    while ((opt = getopt_long(argc, argv, optString, longOpts, &index))
           && opt != -1)
    {
        switch (opt)
        {
        case 'c':
            buffer = hcalloc(sizeof(char), strlen(optarg) + 1);
            strcpy(buffer, optarg);
            c = 1;
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
    if (!c && argc <= optind)
        errx(1, "missing argv"); // errx(1, "missing parameter"); //handel
                                 // reading stdin
    if (!c)
        buffer = file2buf(argv[optind]);
    return exec_42sh(buffer, pretty_print);
}
