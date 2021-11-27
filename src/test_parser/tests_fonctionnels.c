//
// Created by clousty8 on 27/11/2021.
//

#include "../test_parser/print_parser.h"
#include "../parser/parser.h"

int main(void)
{
    struct list *res = build_list("if echo lol \n then \n cat hello -e \n else \n echo truc fi");
    print_list(res);
    return 0;
}
