//
// Created by clousty8 on 24/11/2021.
//

#ifndef INC_42_SH_GRAMAR_STRUCT_H
#define INC_42_SH_GRAMAR_STRUCT_H

enum separator{
    Semi = 0,
    Espe,
    BackS
};

enum operator_{
    AND = 0,
    OR,
    PIPE
};

enum redirect_op{
    RLESS = 0,
    RDGREAT,
    RDLESS,
    RDLESSD,
    RADDGREAT,
    RADDLESS,
    RPIPEGREAT,
    RDSENS,
};



struct input{
    struct list l; // optinal (obviously)
    // linebreak '\n' or EOF
};

struct list{
    struct and_or;
    struct list_next{
        enum separator sep; // !!!!!!! \n pas autorise
        struct  and_or *next;
    };
    enum separator sep; //optionel !!!!! \n pas autorise
};

struct and_or{
    struct pipeline *pipeline;
    struct and_or_next{
        enum operator_ *op;
        // autant qu'on en veut de new line
        struct and_or *next;
    };
};

struct pipeline{
    int negation; //is there an '!'
    struct command *cmd;
    // any number of space
    //optional block{
        //    |  (single pipe)
        //any \n we want
        struct pipeline *pipe_l;
    //};

};

struct command{
    union choose{
        struct simple_command *cmd;
        struct shell_command *sh_cmd;
        struct funcdec *fun;
    };
    struct redirection **list; //possibly null
};

struct simple_command{ // both lists can not be NULL !
    struct prefix **list_pre;
    struct element **list_elt;
};

struct shell_command{
    union choose{
        struct compound_list; // with () or {}
        struct rule_for;
        struct rule_while;
        struct rule_until;
        struct rule_case;
        struct rule_if;
    };
};
struct funcdec{
    char *funct_name;
    // str = ()
    //any \n we want
    struct shell_command *sh_cmd;
};

struct redirection{
    int IONUMBER; // optional
    enum redirect_op re_op;
    char *word; // possibly HEREDOC  for << or <<
};

struct prefix{ // one of the two only
    char *assignment_word;
    struct redirection *re;
};

struct element {
    char *word;
    struct redirection *re;
};

struct compound_list{
    // any \n we want
    struct and_or *and_or;
    struct compound_next{
        enum separator;
        struct compound_list *next;
        //any \n we want
    };
    //the following is optional
    enum separator;
    // any \n we want
};

struct rule_for{
    // for = word detected
    char *word;
    ///*******   can be remplace by ';' semi col
        // any \n we want
        // in token
        char **word_list; // possibly NULL
        enum separator_op *sep_op; // not '&'
    ///*************************************
    //any \n we want
    struct do_group *do_gp;
};

struct rule_while
{
    // token while
    struct compound_list *cp_list;
    struct do_group *do_gp;
};

struct rule_until{
    //token until
    struct compound_list *cp_list;
    struct do_group *do_gp;
};

struct rule_case{
    // token case
    char *word;
    // any \n we want
    // in token
    //any \n we want
    struct case_clause *case_cl; // optional
    //struct Esac; ?? no description of esac
};

struct rule_if{
    // if token
    struct compound_list *cp_list;
    // then token
    struct compound_list *cp_list2;
    struct else_clause *else_cl; // optional
    // fi token
};

union else_clause{
    //else token
    struct compound_list *cp_list;
    //elif token
    struct elif {
        struct compound_list *cp_list2;
        // then token
        struct compound_list *cp_list2bis;
        struct else_clause *next;
    };
};

struct do_group{
    // do token
    struct compound_list *cp_list;
    // done token
};

struct case_clause{
    struct case_item *case_it;
    // ';;' toekn   // last tyhree are optianal but must stay in order
    // any \n we want
    struct case_clause *next;
};

struct case_item{
    // optional '('
    char *word;
    // '|; carac
    char **word_list; // can be NULL but need to be preceded by a '|' carat
    // ')' carac
    // any \n we want
    struct compound_list *cp_list; //optional
};
#endif //INC_42_SH_GRAMAR_STRUCT_H