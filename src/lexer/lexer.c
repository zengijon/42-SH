#include "lexer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

size_t skipspace(const char *input)
{
  size_t i = 0;
  while (input[i] != 0 && (input[i] == '\t' || input[i] == ' '))
    ++i;
  return i;
 
}

int isnumber(const char *s)
{
  for (int i = 0; s[i] != 0 && s[i] != ' '; ++i)
    {
      if (s[i] < '0' || s[i] > '9')
	return 0;
    }
  return 1;
}

int get_number(const char *s)
{
  int i = s[0] - '0';
  for (int j = 1; s[j] != 0 && s[j] != ' '; ++j)
    {
      i = (i * 10) + s[j] - '0'; 
    }
  return i;
}

struct lexer *gestion_quote(struct lexer *lexer, char *input)
{
  size_t i = 1;
  size_t j = 0;
  char *str = calloc(strlen(input), sizeof(char));
  while (input[i] != '\0' && input[i] != '\'')
    {
      str[j++] = input[i++];
    }
  if (input[i] == '\'')
    {
      lexer->current_tok->type = TOKEN_WORDS;
      lexer->current_tok->value = str;
    }
  else
    lexer->current_tok_>type = TOKEN_ERROR;
  return lexer;
}

struct lexer *lexer_new(const char *input)
{
  struct lexer *res = calloc(1, sizeof(struct lexer));
  res->input = input;
  res->pos = skipspace(input);
  res->current_tok = calloc(1, sizeof(struct token));
  if (strncmp(input[res->pos], "if", 2) == 0)
    res->current_tok->type = TOKEN_IF;
  else if (strncmp(input[res->pos], "then", 4) == 0)
      res->current_tok->type = TOKEN_THEN;
  else if (strncmp(input[res->pos], "elif", 4) == 0)
      res->current_tok->type = TOKEN_ELIF;
  else if (strncmp(input[res->pos], "else", 4) == 0)
      res->current_tok->type = TOKEN_ELSE;
  else if (strncmp(input[res->pos], "fi", 2) == 0)
      res->current_tok->type = TOKEN_FI;
  else if (strncmp(input[res->pos], ";", 1) == 0)
      res->current_tok->type = TOKEN_PTCOMA;
  else if (strncmp(input[res->pos], "\n", 1) == 0)
      res->current_tok->type = TOKEN_NEWLINE;
  else if (strncmp(input[res->pos], "'", 1) == 0)
    res = gestion_quote(res, &input[res->pos]);
  else
    {
      size_t j = 0
      size_t k = res->pos;
      char *value = malloc(sizeof(char) * strlen(input));
      while (input[k] != '\0' && input[k] != '\n' && input[k] != ' ')
	  value[j++] = input[k++];
      res->current_tok->type = TOKEN_WORDS;
      res->current_tok->value = value;
    }
  return res;
}


void lexer_free(struct lexer *lexer)
{
  token_free(lexer->current_tok);
}

struct token *lexer_peek(struct lexer *lexer)
{
  char separator = ' ';
  if (lexer->input[lexer->pos] == '\'')
    separator = '\'';
  int i = lexer->pos;
  while(lexer->input[i] != '\0' && lexer->input[i] != separator)
    i++;
  if (lexer->input[i] == '\0')
    {
      return token_new(TOKEN_ERROR);
    }
  i += skipspace(&input[i]);
  struct lexer *tmp = lexer_new(lexer->input + i);
  return tmp->current_tok;
}

struct token *lexer_pop(struct lexer *res)
{
  char separator = ' ';
  if (res->input[res->pos] == '\'')
    separator = '\'';
  struct token *tmp = calloc(1, sizeof(struct token));
  tmp->type = res->current_tok->type;
  tmp->value = res->current_tok->value;
  size_t i = res->pos;
  while(res->input[i] != '\0' && res->input[i] != separator)
    i++;
  const char *input = res->input;
  i += skipspace(&res->input[i]);
  res->pos = i;
  if (res->current_tok == NULL)
    res->current_tok = calloc(1, sizeof(struct token));
  if (strncmp(input[res->pos], "if", 2) == 0)
    res->current_tok->type = TOKEN_IF;
  else if (strncmp(input[res->pos], "then", 4) == 0)
    res->current_tok->type = TOKEN_THEN;
  else if (strncmp(input[res->pos], "elif", 4) == 0)
    res->current_tok->type = TOKEN_ELIF;
  else if (strncmp(input[res->pos], "else", 4) == 0)
    res->current_tok->type = TOKEN_ELSE;
  else if (strncmp(input[res->pos], "fi", 2) == 0)
    res->current_tok->type = TOKEN_FI;
  else if (strncmp(input[res->pos], ";", 1) == 0)
    res->current_tok->type = TOKEN_PTCOMA;
  else if (strncmp(input[res->pos], "\n", 1) == 0)
    res->current_tok->type = TOKEN_NEWLINE;
  else if (strncmp(input[res->pos], "'", 1) == 0)
    res = gestion_quote(res, &input[res->pos]);
  else
    {
      size_t j = 0
	size_t k = res->pos;
      char *value = malloc(sizeof(char) * strlen(input));
      while (input[k] != '\0' && input[k] != '\n' && input[k] != ' ')
	value[j++] = input[k++];
      res->current_tok->type = TOKEN_WORDS;
      res->current_tok->value = value;
    }
  return tmp;
}
