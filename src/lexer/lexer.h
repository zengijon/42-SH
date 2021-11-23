#pragma once

#include "../struct/fifo_lex.h"

struct fifo_lex *build_node(char **buffer);

int get_end(char *buffer);

enum lex get_type(char *buffer, int len);

struct fifo_lex *build_if(char **buffer);
