#pragma once

enum lex
  {
    CMD=0,
    IF,
    ELSE,
    ELIF,
    VAR,
  };

struct fifo_lex
{
  enum lex type;
  char *data;
  struct fifo_lex *l_true;
  struct fifo_lex *r_false;
};

void add_fifo(struct fifo_lex *fifo, enum lex type, char *data);

struct fifo_lex *pop_fifo(struct fifo_lex *fifo);

