#pragma once
#define _GNU_SOURCE //should be removed for mason
#include <fnmatch.h>
#include "token.h"

struct lexer
{
    const char *input; ///< The input data
    size_t pos; ///< The current offset inside the input data
    size_t end; ///< end ok the token
    struct token *current_tok; ///< The next token, if processed
};

struct separator
{
    size_t nb_separator; ///< number of separators
    char **separators; ///< array of separators
};

/**
 * \brief Creates a new lexer given an input string.
 */
struct lexer *lexer_new(const char *input);

/**
 ** \brief Free the given lexer, but not its input.
 */
void lexer_free(struct lexer *lexer);

/**
 * \brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token *lexer_peek(struct lexer *lexer);

//return the n ieme token(n = 1 is equql to lexer_peak)
struct token *lexer_peek_rec(struct lexer *lexer, int n);

/**
 * \brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
struct token *lexer_pop(struct lexer *lexer);
