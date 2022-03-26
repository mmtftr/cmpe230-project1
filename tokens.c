#include "tokens.h"
Token *new_token(TokenType type, char *contents, int line_num)
{
  Token *token = malloc(sizeof(Token));
  token->type = type;
  token->contents = contents;
  token->line_num = line_num;
  return token;
}

void free_tokens(Token **token_arr)
{
  for (int i = 0; token_arr[i] != NULL; i++)
  {
    free(token_arr[i]);
  }
}