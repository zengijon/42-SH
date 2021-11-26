//
// Created by jennie on 24/11/2021.
//
#define _XOPEN_SOURCE >= 500

#include "parser.h"
#inbclude "???" // chemin pour le lexer
#include "../memory/hmalloc.h"

#include <string.h>
#include <err.h>



// weird grammar not chou
struct rule_for *build_rule_for(struct lexer *lex)
{
    struct rule_for *res = hcalloc(1, sizeof(struct relu_for));

    if ( ! /*lexer | seek | reçu == for */)
        return NULL;
    // lexer | pop

    if ( ! /*lexer | seek | reçu != char *word*/)
        errx(1, "missing word after for");

    res->word = ""; /* lexer | pop | attendu : char *word*/

    if (/*lexer | seek | attendu : ;*/)
    {
        res->sep = Semi; /* lexer | pop | attendu : ;*/
        is_good = 1;
    }
    else if (/*lexer | seek | attendu : char **word_list*/)
    {
        while (/*lexer | seek | attendu : \n*/)
            ;/*lexer | pop */


        res->word_list = NULL; /*lexer | pop | attendu : char * word_list*/
        if ( /*lexer | seek | attendu : sep_op not &*/)
        {
            res->sep_op = 0; /*lexer | pop | attendu : sep_op but not &*/
            is_good = 1;
        }
        else
            errx(1, "not got sep_op which is not &");
    }


    if (is_good)
    {
        if (/*lexer | seek | attendu : do_group*/)
        {
            res->do_gp = build_do_group(lex);
            if (res->do_gp == NULL)
                errx(1, "Failed build_do_gp");
        }
    }
    errx(1, "do not got ; or word_list");
}

struct rule_while *build_while(struct lexer *lex)
{
    struct rule_while *res = hcalloc(1, sizeof(struct rule_while));
    if (res == NULL)
        errx(1, "Calloc failed");

    if ( !/* lexer | seek | attendu : while*/)
        return NULL;

    /* lexer | pop  */
    if (/* lexer | seek | attendu : compound_list*/)
    {
        res->cp_list = build_compound_list(lex); /* lexer | pop */

        if (res->cp_list == NULL)
            errx(1, "Failed build_compound_list");

        if (/*lexer | seek | attendu : do_group */)
        {
            res->do_gp = build_do_group(lex); /* lexer | pop */

            if (res->do_gp == NULL)
                errx(1, "Failed build_go_group");

            return res;
        }
        errx(1, "Missing do_group");
    }
    errx(1, "missing compound_list":);
}

struct rule_until *build_until(struct lexer *lex)
{
    struct rule_until *res = hcalloc(1, sizeof(struct rule_until));

    if (res == NULL)
        errx(1, "Calloc failed");

    if (!/* lexer | seek | attendu : until*/)
        return NULL;

    /* lexer | pop */
    if (/* lexer | seek | attendu : compound_list*/)
    {
        res->cp_list = build_compound_list(lex); /* lexer | pop */
        if (res->cp_list == NULL)
            errx(1, "build_compound_list failed");

        if (/* lexer | seek | attendu : do-group*/)
        {
            res->do_gp = build_do_group(lex); /* lexer | pop */

            if (res->do_gp == NULL)
                errx(1, "Build_do_group failed");

            return res;
        }
        errx(1, "missing do_group in until");
    }
    errx(1, "compound_l;ist missing");
}

struct rule_case *build_case(struct lexer *lex)
{
    struct rule_case 8res = hcalloc(1, sizeof(struct rule_case));

    if (res == NULL)
        errx(1, "Calloc failed");

    if (!/* lexer | seek | attendu : case */)
        return NULL;

    /* lexer | pop */
    if (/* lexer | seek | attendu : word*/)
    {
        res->word = strdup(" "); // lexer | pop

        while (/*lexer | seek | attendu : \n*/)
            ;/*lexer | pop */

        if (! /* lexer | seek | attendu : in */)
            errx(1, "Missing in in case");
        //lexer | pop

        while (/*lexer | seek | attendu : \n*/)
            ;/*lexer | pop */

        if (/*lexer | seek | attendu : case_clause*/)
        {
            res->case_clause = build_case_clause(lex); //lexer | pop

            if (res->case_clause == NULL)
                errx(1, "build_case_clause failed");
        }

        if (! /*lexer | seek | attendu : esac*/)
            errx(1, "Missing Esac in case");

        //res->esac = ??? //lexer |pop

        return res;
    }
    errx(1, "Missing word after case");
}

struct rule_if *build_rule_if(struct lexer *lex)
{
    struct rule_if *res = hcalloc(1, sizeof(struct rule_if));

    if (/* lexer | seek | attendu != if*/)
        return NULL;
    // lexer | pop

    res->cp_list = build_compound_list(lex);

    if (res->cp_list == NULL)
        errx(1, "Missing compound_list in if");

    if (/*lexer |seek | reçu != then */)
        errx(1, "then token is missing");
    // lexer | pop

    if ((res->cp_list2 = build_compound_list(lex) == NULL))
        errx(1, "missing compound_list in if");

    res->else_cl = build_else_clause(lex);

    if (/* lexer | seek | if != token fi*/)
        errx(1, "missing fi");
    // lexer | pop
    return res;
}

struct else_clause *build_else_clause(struct lexer *lex)
{
    struct else_clause *res = hcalloc(1, sizeof(union else_clause));

    if (/* lexer |seek |attendu : else */)
    {
        if ((res->cp_list = build_compound_list(lex) != NULL))
            return res;
        errx(1, "Missing compound_list");
    }
    if (/*lexer |seek | attendu : elif*/)
    {
        res->elif.cp_list2 = build_compound_list(lex);

        if (res->elif.cp_list2 == NULL)
            errx(1, "Missing compound_list in if");

        if (/*lexer |seek | reçu != then */)
            errx(1, "then token is missing");
        // lexer | pop

        if ((res->elif.cp_list2bis = build_compound_list(lex) == NULL))
            errx(1, "missing compound_list in if");

        res->elif.next = build_else_clause(lex);

        return res;
    }
    return NULL;
}

struct do_group *build_do_group(struct lexer *lex)
{
    struct do_group *res = hcalloc(1, sizeof(struct do_group));
    if (res == NULL)
        errx(1, "Calloc failed");

    if (!/*lexer |seek | attendu : do */)
        return NULL;

    if (/*lexer | seek | attendu : compound_list*/)
    {
        res->cp_list = build_compound_list(lex); // lexer | pop

        if (res->cpo_list == NULL)
            errx(1, "build_compound_list failed");

        if (! /* lexer | seek | attendu : token Done*/)
            errx(1, " Missing token done");
        return res;
    }
    errx(1, "Missing compound_list after do for do_group");
}

static struct case_clause_bis *build_case_clause_bis(struct lexer *lex)
{
    struct case_clause_bis *res = hcalloc(1, sizeof(struct case_clause_bis));
    if (res == NULL)
        errx(1, "calloc failed");
    if (/*lexer |seek |attendu : ;;*/)
    {
        res->is_double_semi = 1;

        while (/*lexer | seek |atten du : \n*/)
            ; //lexer | pop

        if (/*lexer |seek | attendu : case_item*/)
        {
            res->case_it = build_case_item(lex); // lexer |pop

            if (res->case_it == NULL)
                errx(1, "build_case_item failed");
            return res; // pb avec l'etoile est ce au'il faut la repeter plusieurs fois avec une boucle while ou juste rec
        }
        errx(1, "missing case_item afeter ;;");
    }
    return NULL;
}

struct case_clause *build_case_clause(struct lexer *lex)
{
    struct case_clause *res =hcalloc(1, sizeof(struct cause_clause));
    if (res == NULL)
        errx(1, "calloc failed");

    if (/*lexer | seek | attendu : case_item*/)
    {
        res->case_it = build_case_item(lex); //lexer | pop

        if (res->case_it == NULL)
            errx(1, "build_case_item failed");

        res->next = build_case_clause_bis(lex); // lexer | pop
        //meme si c'est nul c'est pas grave car on a une etoile => donc l'etoile peut juste etre nulle
        if (/*lexer | seek |attendu : ;;*/)
            res->is_double_semi = 1;

        while (/*lexer |seek | attendu : \n*/)
            ;//lexer | pop

        return res;
    }
    return NULL;
}

static char **word_list_pipe(struct lexer *lex)
{
    char **res = hcalloc(1, sizeof(char *));
    char *temp = hcalloc(2048, sizeof(char));
    int is_once = 0;
    if (res == NULL && temp == NULL)
        errx(1, "calloc failed");

    while (/*lexer |seek | attendu : |*/)
    {
        is_once = 1;
        temp = strcat(temp, "|");
        if (/*lexer |seek | attendu : word*/)
            temp = strcat(temp, " ");// y mettre lexer |pop
        else
            errx(1, "missing word after |");
    }
    return is_once == 1 ? temp : NULL;
}

struct case_item *build_case_item(struct lexer *lex)
{
    struct case_item *res = hcalloc(1, sizeof(struct case_item));
    if (res == NULL)
        errx(1, "calloc failed");

    int is_smth = 0;
    if (/*lexer | seek | attendu : (*/)
    {
        is_smth = 1;
        res->is_open_bracket = 1;
    }

    if (/*lexer |seek | attendu : word*/)
    {
        res->word = strdup(" "); // lexer | pop

        res->word_list = word_list_pipe(lex);

        if (res->word_list == NULL)
            errx(1, "word_list_pipe failed");

        while (/*lexer |seek | attendu : \n*/)
            ; // lexer | pop

        if (/*lexer | seek | attendu : compound_list*/)
        {
            res->cp_list = build_compound_list(lex); // lexer | pop

            if (res->cp_list == NULL)
                errx(1, "build_compound_list failed");
        }
        return res;
    }
    if (is_smth)
        errx(1, "missing word after (");
    else
        return NULL;
}
