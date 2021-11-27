//
// Created by clousty8 on 27/11/2021.
//

#include "../test_parser/print_parser.h"
#include "../parser/parser.h"

#include <stdio.h>

int main(void)
{
    //struct list *res = build_list("if echo lol \n then \n cat hello -e \n else \n echo truc fi");
    //print_list(res);

    //struct list *res = build_list("if echo lol then\n cat -e hello\n echo lol1 fi");
    //print_list(res);

    //struct list * res2 = build_list("echo machin \n echo truct \n");
    //print_list(res2);

    //struct list *res = build_list("if [ echo 1 && echo 3 ] \n then echo 2 fi"); PB !!!
    //struct list *res = build_list("echo 1 && echo 2"); ne marche pas
    printf("==============Test===============\n");
    struct list *res = build_list("\n\n\n\n\n\n\n\necho lol;\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //marche pas
    print_list(res);
    printf("\n\n\n");


    printf("==============Test===============\n");
    struct list *res1 = build_list("\n\n\n\n\n\n\n\nif echo 1 \n then echo 2 fi\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //marche pas
    print_list(res1);
    printf("\n\n\n");

    printf("==============Test===============\n");
    struct list *res2 = build_list("echo 1  \n\n\n\n\n\n\n\n\n"); //marche pas
    print_list(res2);
    printf("\n\n\n");

    printf("==============Test===============\n");
    struct list *res3 = build_list("if echo 2 \n\n\n\n\n\n then \n fi"); //marche pas
    print_list(res3);
    printf("\n\n\n");

    return 0;
}
