#include "token.h"

#include <err.h>
#include <stdlib.h>

struct token *token_new(enum token_type type)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = type;
    return new;
}

void token_free(struct token *token)
{
    free(token);
}