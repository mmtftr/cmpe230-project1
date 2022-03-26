#pragma once
#include "tokens.h"

void get_separated_form(char *result, char *expression);
char **get_tokens_to_scan(char *expression);
Token *get_tokens_from_line(char *line, int line_num);
TokenType get_token_type(char *token);