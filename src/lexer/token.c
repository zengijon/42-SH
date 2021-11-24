#include "token.h"

#include <err.h>
#include <stdlib.h>

#include "hmalloc.h"

struct token *token_new(enum token_type type)
{
    struct token *new = hmalloc(sizeof(struct token));
    new->type = type;
    return new;
}

void token_free(struct token *token)
{
    hfree(token);
}
