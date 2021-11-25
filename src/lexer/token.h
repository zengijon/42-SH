#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF, ///< 'if'-> 0
    TOKEN_THEN, ///< 'then' -> 1
    TOKEN_ELIF, ///< 'elif'-> 2
    TOKEN_ELSE, ///< 'else' -> 3
    TOKEN_FI, ///< "fi" -> 4
    TOKEN_PTCOMA, ///< ';' -> 5
    TOKEN_NEWLINE, ///< '\n' -> 6
    TOKEN_QUOTE, ///< ''' -> 7
    TOKEN_WORDS, ///< every words -> 8
    TOKEN_ERROR ///< tt's not a real token, it's returned in case of invalid -> 9
};

struct token
{
    enum token_type type; ///< The kind of token
    char *value; ///< If the token are words, its value
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);
