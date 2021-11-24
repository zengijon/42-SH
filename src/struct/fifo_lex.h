#pragma once

enum lex
{
    CMD=0,
    IF,
    VAR,
    OR,
    AND,
    TOKEN
};

struct fifo_lex
{
    enum lex type;
    union content
    {
        struct fifo_lex *condition;
        char *data;
    };
    struct fifo_lex *l_true;
    struct fifo_lex *r_false;
};

void add_fifo(struct fifo_lex *fifo, enum lex type, char *data);

struct fifo_lex *pop_fifo(struct fifo_lex *fifo);
