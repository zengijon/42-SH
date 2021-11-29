#pragma once

//skip spaces
size_t skipspace(const char *input);

//create token_word with all the string between the quotes
struct lexer *gestion_quote(struct lexer *lexer, const char *input);

//create an array of strings with all the separators
struct separator *build_separator_list(void);

//determine if a string is a separator or not
int is_separator(const char *input, struct separator *separator);

//determine if a string is a token or not
int is_token(const char *input, char *token, int n);

//determine if a string is a simple '&', a simple '|', a 'or' or a 'and'
struct lexer *gestion_and_or(struct lexer *lexer, const char *input);
