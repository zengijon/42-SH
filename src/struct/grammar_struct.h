//
// Created by clousty8 on 24/11/2021.
//

#ifndef INC_42_SH_GRAMAR_STRUCT_H
#define INC_42_SH_GRAMAR_STRUCT_H

enum operator_
{
    AND = 1,
    OR
};

enum redirect_op
{
    RLESS = 1,
    RDGREAT,
    RDLESS,
    RDLESSD,
    RADDGREAT,
    RADDLESS,
    RPIPEGREAT,
    RDSENS,
};

struct list_next
{ // optional
    struct and_or *a_o;
    int esp; // !!!!!!! \n pas autorise
    struct list_next *next;
};

struct list
{
    struct and_or *a_o;
    int esp; // optionel ; ou &
    struct list_next *next; // optional
};

struct and_or_next
{ // optional
    enum operator_ op;
    // autant qu'on en veut de new line
    struct pipeline *pipeline;
    struct and_or_next *next; // optional
};

struct and_or
{
    struct pipeline *pipeline;
    struct and_or_next *next; // optional
};

struct pipeline_next
{
    // |
    // any \n we want
    struct command *cmd;
    struct pipeline_next *next;
};

struct pipeline
{
    int negation; // is there an '!'
    struct command *cmd;
    // optional block
    struct pipeline_next *next;
};

struct command
{
    int nb_redir;
    struct redirection **redir; // possibly null
    struct simple_command *s_cmd; // only one of the three plus redirection if
                                  // the first was not choosen
    struct shell_command *sh_cmd;
    struct funcdec *fun;
    int nb_redir2;
    struct redirection **redir2; // possibly null
};

struct simple_command
{ // both lists can not be NULL !
    int size_pre; // one of them != 0
    int size_elt;
    struct prefix **list_pre;
    struct element **list_elt;
};

struct shell_command
{
    int is_subshell;
    struct compound_list *c_p; // with () or {}
    struct rule_for *r_f;
    struct rule_while *r_w;
    struct rule_until *r_u;
    struct rule_case *r_c;
    struct rule_if *r_i;
};

struct funcdec
{
    char *funct_name;
    // str = ()
    // any \n we want
    struct command *sh_cmd;
};

struct redirection
{
    char *redir_type;
    char *word;
};

struct prefix
{ // one of the two only
    char *assignment_word;
    struct redirection *redirect;
};

struct element
{ // one of the two only
    char *word;
    struct redirection *redirect;
};

struct compound_next
{
    // any \n we want
    struct and_or *a_o;
    struct compound_next *next; // as many we want
    int esp; // optional
};

struct compound_list
{
    // any \n we want
    struct and_or *and_or;
    int esp; // optional

    struct compound_next *next;
};

struct rule_for
{
    // for = word detected
    char *word;
    // int flag_semi_col; // token ;
    ///*******   can be remplace by ';' semi col
    // any \n we want
    // in token
    int wl_s;
    char **word_list; // possibly NULL
    // / rajouter une size_word_list
    // not '&'
    ///*************************************
    // any \n we want
    struct do_group *do_gp;
};

struct rule_while
{
    // token while
    struct compound_list *cp_list;
    struct do_group *do_gp;
};

struct rule_until
{
    // token until
    struct compound_list *cp_list;
    struct do_group *do_gp;
};

struct rule_case
{
    // token case
    char *word;
    // any \n we want
    // in token
    // any \n we want
    struct case_clause *case_cl; // optional
    // token Esac;
};

struct rule_if
{
    // if token
    struct compound_list *cp_list;
    // then token
    struct compound_list *cp_list2;
    struct else_clause *else_cl; // optional
    // fi token
};

struct else_clause
{
    // else token
    struct compound_list *cp_list;

    // elif token
    struct compound_list *cp_list2;
    // then token
    struct compound_list *cp_list2bis;
    struct else_clause *next;
};

struct do_group
{
    // do token
    struct compound_list *cp_list;
    // done token
};

struct case_clause_bis
{
    // semi
    // any \n we want
    struct case_item *case_it;
};

struct case_clause
{
    struct case_item *case_it;
    struct case_item **next;
    int next_size;
    // optional ;;
    // any \n we want
};

struct case_item
{
    //possible open paren
    char **word_list; // can not be NULL but need to be preceded by a '|' carat
    int w_l_size;
    /// rajouter une size pour la word list
    // ')' carac
    // any \n we want
    struct compound_list *cp_list; // optional
};
#endif // INC_42_SH_GRAMAR_STRUCT_H
