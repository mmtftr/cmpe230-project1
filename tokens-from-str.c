#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "tokens.h"
// FILE IS DEPRECATED!!!

Token *get_tokens_from_file(char *filename)
{
  FILE *fp;
  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Cannot open %s\n", argv[1]);

    return NULL;
  }
  char line[256];
  int tokens_index = 0;
  Token *tokens = calloc(LINE_LIMIT * LINE_LIMIT, sizeof(Token));

  int line_num = 1;
  while (fgets(line, LINE_LIMIT, fp) != NULL)
  {
    Token *some_tokens = get_tokens_from_line(line, line_num);
    for (int j = 0; some_tokens[j].type != TKN_LINE_FEED; j++)
    {
      tokens[tokens_index] = some_tokens[j];
      tokens_index++;
    }
    some_tokens[tokens_index] = some_tokens[j];
    tokens_index++;
    line_num++;
    free(some_tokens);
  }

  fclose(fp);
  return tokens;
}
void get_separated_form(char *result, char *expression)
{
  int result_loc = 0;

  int last_isalnum = 0;
  int last_isspace = 1;

  int should_place_space = 0;
  for (int i = 0; expression[i] != '\0'; i++)
  {
    if (expression[i] == '#')
    {
      break;
    }
    should_place_space = (isalnum(expression[i]) && !last_isalnum) || (!isalnum(expression[i]) && expression[i] != ' ');

    should_place_space = should_place_space && !last_isspace;

    if (last_isspace && expression[i] == ' ')
    {
      continue;
    }

    if (should_place_space)
    {
      result[result_loc] = ' ';
      result_loc++;
      result[result_loc] = expression[i];
      result_loc++;
    }
    else
    {
      result[result_loc] = expression[i];
      result_loc++;
    }
    last_isalnum = isalnum(expression[i]);
    last_isspace = expression[i] == ' ';
  }
  result[result_loc] = '\0';
}

char **get_tokens_to_scan(char *expression)
{
  char **tokens = calloc(N, sizeof(char *));
  int tokens_index = 0;

  char separated_form[N];
  get_separated_form(separated_form, expression);

  char *dup_string = strdup(separated_form);
  char *token = strsep(&dup_string, " ");
  while (token != NULL)
  {
    if (strcmp(token, "") != 0)
    {
      tokens[tokens_index++] = token;
    }
    token = strsep(&dup_string, " ");
  }
  tokens[tokens_index] = "$";
  return tokens;
}

Token *get_tokens_from_line(char *line, int line_num)
{
  char **tokens = get_tokens_to_scan(line);
  Token *token_list = calloc(LINE_LIMIT, sizeof(Token));
  int token_list_index = 0;

  for (int i = 0; strcmp(tokens[i], "$") != 0; i++)
  {
    if (strcmp(tokens[i], "") != 0)
    {
      TokenType type = get_token_type(tokens[i]);
      if (type == = TKN_UNKNOWN)
      {
        free(token_list);
        return NULL;
      }
      token_list[token_list_index].type =
          token_list[token_list_index].contents = strdup(tokens[i]);
      token_list[token_list_index].line_num = line_num;

      token_list_index++;
    }
  }
  token_list[token_list_index].type = TKN_LINE_FEED;
  token_list[token_list_index].contents = strdup("\n");
  token_list[token_list_index].line_num = line_num;
  return token_list;
}

TokenType get_token_type(char *token)
{
  if (strcmp(token, "for") == 0)
  {
    return TKN_KW_FOR;
  }
  else if (strcmp(token, "in") == 0)
  {
    return TKN_KW_IN;
  }
  else if (strcmp(token, "(") == 0)
  {
    return TKN_PN_OPENPAREN;
  }
  else if (strcmp(token, ")") == 0)
  {
    return TKN_PN_CLOSEPAREN;
  }
  else if (strcmp(token, ".") == 0)
  {
    return TKN_PN_COMMA;
  }
  else if (strcmp(token, ",") == 0)
  {
    return TKN_PN_COMMA;
  }
  else if (strcmp(token, ":") == 0)
  {
    return TKN_PN_COLON;
  }
  else if (strcmp(token, "{") == 0)
  {
    return TKN_PN_OPENBRACE;
  }
  else if (strcmp(token, "}") == 0)
  {
    return TKN_PN_CLOSEBRACE;
  }
  else if (strcmp(token, "+") == 0)
  {
    return TKN_PLUS;
  }
  else if (strcmp(token, "*") == 0)
  {
    return TKN_MULT;
  }
  else if (strcmp(token, "-") == 0)
  {
    return TKN_MINUS;
  }
  else if (strcmp(token, "scalar") == 0)
  {
    return TKN_TYPE_SCALAR;
  }
  else if (strcmp(token, "vector") == 0)
  {
    return TKN_TYPE_VECTOR;
  }
  else if (strcmp(token, "matrix") == 0)
  {
    return TKN_TYPE_MATRIX;
  }
  else
  {
    if (isnumber(token[i]))
    {
      return TKN_NUMBER_LITERAL;
    }
    else if (isalpha(token[i]))
    {
      return TKN_IDENT;
    }
    else
    {
      return TKN_UNKNOWN;
    }
  }
}
