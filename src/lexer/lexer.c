#include "lexer.h"

#include <err.h>
#include <string.h>

int find_c(char c, char *buffer)
{
  int i = 0;
  for (; buffer[i] != 0 && buffer[i] != c; ++i)
    {
      if (buffer[i] == '\\')
	++i;
    }
  return i;
}

int skip_space(char *buffer)
{
  int i = 0;
  while (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n')
    ++i;
  return i;
}

int get_end(char *buffer)
{
  int res = skip_space(buffer);
  while (buffer[res] != 0 && buffer[res] != ';' && buffer[res] != '\n')
    {
      if (buffer[res] == '\'' || buffer[res] == '"')
	{
	  res += find_c(buffer[res], buffer + res + 1);

	  //A vérifier
	  
	  if (buffer[res] == 0)
	    errx(1, "Guillemet pas fermé");
	}
      if (buffer[res] == '\\')
	res++;
      res++;
    }
  return res;
}

//Trop simplifié
int isvar(char *buffer, int len, int i)
{
  for (; i < len; ++i)
    {
      if (buffer[i] == '\\')
	++i;
      if (buffer[i] == '=')
	return 1;
    }
  return 0;
}

enum lex get_type(char *buffer, int len)
{
  int i = skip_space(buffer);
  if (isvar(buffer, len, i))
    return VAR;
  if (strncmp(buffer + i, "if ", 3))
    return IF;
  if (strncmp(buffer + i, "else ", 5))
    return ELSE;
  if (strncmp(buffer + i, "elif ", 5))
    return ELIF;
  return CMD;
}

struct fifo_lex *build_node(char **buffer)
{
  struct fifo_lex *fifo = hcalloc(1, sizeof(struct fifo_lex));
  fifo->type = get_type(*buffer, get_len(*buffer));

  switch (fifo->type)
    {
    case IF:
      return build_if(buffer);
    case ELSE:
      
    }
}

