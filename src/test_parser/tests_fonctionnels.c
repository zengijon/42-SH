//
// Created by clousty8 on 27/11/2021.
//

#include "../test_parser/print_parser.h"
#include "../parser/parser.h"

int main(void)
{
    struct list *res = build_list("echo lol");
    print_list(res);
    return 0;
}
