//
// Created by clousty8 on 24/11/2021.
//

#ifndef INC_42_SH_GRAMAR_STRUCT_H
#define INC_42_SH_GRAMAR_STRUCT_H

enum grammar_symbol{
    word = 0,
    assignement_word,
    name,
    new_line,
    ionumber
};

enum operator_{
    AND_IF = 0,
    OR_IF,
    DSEMI,
    DLESS,
    DGREAT,
    DLESSAND,
    GREATAND,
    LESSGREAT,
    DLESSDASH
    CLOBBER
};

enum reserved_word{
    If = 0,
    Then
    Else,
    Elif,
    Fi,
    Do,
    Done,
    Case,
    Esac,
    While,
    Until,
    For,
    Lbrace,
    Rbrace,
    Bang,
    In
};

enum separator_op{
    Esper = 0, // stands for '&'
    Dotcoma // stands for ';'
};

struct program{
        struct complete_command * c_cmd;
}; //most likely not usefull at all

struct complete_command{
    struct list *l;
    enum separator_op;
    struct complete_command_next {
        struct newlist *sep; // can NOT be NULL
        struct complete_command *next;
    };
};

struct list
{
    struct and_or;
    struct list_next{
        enum separator_op;
        struct list *next;
    };
};


struct and_or{
    struct pipeline *pi;
    struct and_or_next{
        struct and_or *next;
        enum operator_token; // only AND_IF & OR_IF are allowed ? may need special enum
        struct linebreak;
        struct pipeline;
    };
};

struct pipeline{
    struct bang *b; // possibly unused
    struct pipesequence *piseq;
};

struct pipesequence{
    union command *cmd;
    struct pipesequence_next{
        // needss pipe '|'
        struct linebreak;
        struct pipesequence *next;
    };
};

union command{
    struct simple_command *s_cmd;
    union compound_command *cp_cmd;
    struct compound_command_redirect_list *cp_cmd_r_l;
    struct function_definition *f_def;
};

union compound_command{
    struct brace_group *brace_gp;
    struct subshell *subsh;
    struct for_clause *for_cl;
    struct case_clause *case_cl;
    struct if_clause *if_cl;
    struct while_clause *while_cl;
    struct until_clause *until_cl;
};

struct subshell{
    //parenthese
    union compound_list *cp_list; // entre parentheses focement
    //parenthese
};

union compound_list{
    struct linebreak *lb;
    struct term *term;
    struct separator *sep;
};

struct term{
    struct term_next {
        struct term *term;
        struct separator sep;
    };
    struct and_or *a_o;
};

struct for_clause{
    // token for
    struct *name;
    struct *do_group;
    /// the following is not mandatory | the order maters
    struct *sequential_sep; // if yes the following can be part of it or not if the order is not respected
    struct *linebreak;
    struct *In;
    // token IN ?
    struct *worldlist;
};

struct name{
    // rule 5
};

struct In{

};
#endif //INC_42_SH_GRAMAR_STRUCT_H