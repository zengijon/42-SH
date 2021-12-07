#include <err.h>
#include <getopt.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "../exec/exec.h"
#include "../memory/hmalloc.h"
#include "../parser/parser.h"
#include "../test/test_parser/print_parser.h"
#include "../utils/file2buf.h"
#include "stdio.h"

struct free_list *list_malloc = NULL;

char *my_itoa(int n, char *s)
{
    int k = 0;
    if (n < 0)
    {
        s[k++] = '-';
        n = -1 * n;
    }
    int i = 1;
    for (int j = n; j >= 10; j /= 10)
        i *= 10;
    for (; i > 0; i /= 10)
    {
        s[k++] = '0' + (n / i);
        n %= i;
    }
    s[k] = 0;
    return s;
}

char *get_value_in_vl(struct exec_struct *e_x, char *name)
{
    for (int i = 0; i < e_x->v_l_size; i++)
        if (strcmp(name, e_x->v_l[i].name) == 0)
            return e_x->v_l[i].value;
    return NULL;
}

// char *get_path(void)
//{
//    char s[2048] = { 0 };
//    getcwd(s, 2048);
//    if (s == NULL)
//        errx(1, "getcwd failed");
//    char *res = hcalloc(1, strlen(s));
//
//    memccpy(res, s, strlen(s));
//
//    return res;
//}

struct exec_struct *build_exec_struct(int argc, char **argv)
{
    char *A_starval = hcalloc(15536, 1);
    struct exec_struct *e_x = hcalloc(1, sizeof(struct exec_struct));
    for (int i = optind; i <= argc - optind; ++i)
    {
        assign_var(my_itoa(i - optind, hcalloc(1, 8)), argv[i], e_x);
        if (i != optind)
        {
            strcat(A_starval, " ");
            strcat(A_starval, argv[i]);
        }
    }
    assign_var("*", A_starval, e_x);
    assign_var("#", my_itoa(argc - optind - 1, hcalloc(1, 8)), e_x);
    assign_var("$", my_itoa(getpid(), hcalloc(1, 8)),
               e_x); // assign_var("OLDPWD", get_path(),e_x);
    // assign_var("PWD", get_path(), e_x);
    return e_x;
}

int exec_42sh(char *buffer, int pretty_print, struct exec_struct *e_x)
{
    int res = 0;
    struct list *list;
    struct lexer *lex =
        lexer_new(expand_special_var(buffer, get_value_in_vl(e_x, "*")));

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
    if (argc == 1)
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
    return exec_42sh(buffer, pretty_print, build_exec_struct(argc, argv));
}
