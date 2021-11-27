//
// Created by clousty8 on 24/11/2021.
//

#ifndef INC_42_SH_GRAMAR_STRUCT_H
#define INC_42_SH_GRAMAR_STRUCT_H

enum separator_
{
    Semi = 1,
    Espe,
    BackS
};

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
    enum separator_ sep; // !!!!!!! \n pas autorise
    struct and_or *a_o;
    struct list_next *next;
};

struct list
{
    struct and_or *a_o;
    struct list_next *next; // optional
    enum separator_ sep; // optionel !!!!! \n pas autorise
};

struct and_or_next
{ // optional
    enum operator_ *op;
    // autant qu'on en veut de new line
    struct pipeline *pipeline; // pas mis avant et avant c'etait and_or *next
                               // mais pose pb pour l'implementation
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
    struct simple_command *cmd;
    struct shell_command *sh_cmd;
    struct funcdec *fun;
    struct redirection *redir; // possibly null
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
    struct shell_command *sh_cmd;
};

struct redirection
{
    int IONUMBER; // optional
    enum redirect_op re_op;
    char *word; // possibly HEREDOC  for << or <<
    struct redirection *next;
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
    enum separator_ sep;
    // any \n we want
    struct and_or *a_o;
    struct compound_next *next; // as many we want
};

struct compound_list
{
    // any \n we want
    struct and_or *and_or;

    struct compound_next *next;
};

struct rule_for
{
    // for = word detected
    char *word;
    int flag_semi_col; // token ;
    ///*******   can be remplace by ';' semi col
    // any \n we want
    // in token
    char **word_list; // possibly NULL
    enum separator_ *sep_op; // not '&'
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
    // struct Esac; ?? no description of esac
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
    int is_double_semi;
    // any \n we want
    struct case_item *case_it;
};

struct case_clause
{
    struct case_item *case_it;
    struct case_clause_bis *next;
    // optional ;;
    int is_double_semi;
    // any \n we want
};

struct case_item
{
    int is_open_bracket;
    char *word;
    // '|; carac
    char **word_list; // can be NULL but need to be preceded by a '|' carat
    // ')' carac
    // any \n we want
    struct compound_list *cp_list; // optional
};
#endif // INC_42_SH_GRAMAR_STRUCT_H
