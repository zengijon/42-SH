#pragma once

#include <unistd.h>

enum token_type
{
    TOKEN_IF, ///< 'if'-> 0
    TOKEN_THEN, ///< 'then' -> 1
    TOKEN_ELIF, ///< 'elif'-> 2
    TOKEN_ELSE, ///< 'else' -> 3
    TOKEN_FI, ///< 'fi' -> 4
    TOKEN_WHILE, ///< 'while' -> 5
    TOKEN_FOR, ///< 'for' -> 6
    TOKEN_DO, ///< 'do' -> 7
    TOKEN_DONE, ///, 'done' -> 8
    TOKEN_PTCOMA, ///< ';' -> 9
    TOKEN_NEWLINE, ///< '\n' -> 10
    TOKEN_QUOTE, ///< ''' -> 11
    TOKEN_WORDS, ///< every words -> 12
    TOKEN_EOF, ///< EOF -> 13
    TOKEN_PIPE, ///<'|'-> 14
    TOKEN_OR, ///< '||' -> 15
    TOKEN_ESP, ///< '&' -> 16
    TOKEN_AND, ///<'&&' -> 17
    TOKEN_UNTIL, ///<'until' -> 18
    TOKEN_NEG, ///<'!' -> 19
    TOKEN_REDIR, /// <> -> 20
    TOKEN_PA_OPEN, /// '(' -> 21
    TOKEN_PA_CLOSE, /// ')' -> 22
    TOKEN_ACO_OPEN,///< '{' -> 23
    TOKEN_ACO_CLOSE,///< '}' -> 24
    TOKEN_CASE, ///< 'if'-> 25
    TOKEN_ESAC, ///< 'then' -> 26
    TOKEN_ERROR ///< it's not a real token, it's returned in case of invalid -> 27
};

// drfhfjfgj
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
