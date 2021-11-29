//
// Created by clousty8 on 27/11/2021.
//

#include <stdio.h>

#include "../../parser/parser.h"
#include "print_parser.h"

int main(void)
{
    // struct list *res = build_list("if echo lol \n then \n cat hello -e \n
    // else \n echo truc fi"); print_list(res);

    // struct list *res = build_list("if echo lol then\n cat -e hello\n echo
    // lol1 fi"); print_list(res);

    // struct list * res2 = build_list("echo machin \n echo truct \n");
    // print_list(res2);

    // struct list *res = build_list("if [ echo 1 && false ] \n then echo 2
    // fi"); PB !!! struct list *res = build_list("echo 1 && echo 2"); ne marche
    // pas
    printf("==============Test1===============\n");
    struct list *res =
        build_list("\n\n\n\n\n\n\n\necho lol;\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    print_list(res);
    printf("\n\n\n");

    printf("==============Test2===============\n");
    struct list *res1 = build_list("\n\n\n\n\n\n\n\nif echo 1 \n then echo 2 "
                                   "fi\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    print_list(res1);
    printf("\n\n\n");

    printf("==============Test3===============\n");
    struct list *res2 = build_list("echo 1  \n\n\n\n\n\n\n\n\n");
    print_list(res2);
    printf("\n\n\n");

    printf("==============Test4===============\n");
    struct list *a =
        build_list("if if echo truc \n then \n asd fi \n then \n asd \n fi");
    print_list(a);
    printf("\n\n\n");

    printf("==============Test5===============\n");
    struct list *res3 =
        build_list("if echo 2 \n\n\n\n\n\n then \n yjtfy fi"); // marche pas
    print_list(res3);
    printf("\n\n\n");

    printf("==============Test6===============\n");
    struct list *res4 =
        build_list("if echo 2 \necho 4\n\nif truc \n then lol fi \n then machin \n fi\necho "
                   "5\n\n then \n yjtfy \n fi"); // marche pas
    print_list(res4);
    printf("\n\n\n");
    return 0;
}
