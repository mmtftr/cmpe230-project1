#pragma once
#include "tokens.h"

struct Scanner
{
  char *contents;
  int line_num;
  int pos;
  int len;

  // If an error happened during scanning, this will be NULL
  // with the line number set to where the error happened.
  Token *scanned_tokens;
};

typedef struct Scanner Scanner;
void scanner_init(Scanner *scanner, char *contents, int line_num);
char advance(Scanner *scanner);
void go_back(Scanner *scanner);
char peek(Scanner *scanner);
char peek_next(Scanner *scanner);
char peek_prev(Scanner *scanner);
int match(Scanner *scanner, char c);
Token *get_tokens_from_file(char *filename);
Scanner *scan_str(char *str);
Token *get_tokens_with_scanner(Scanner *scanner);
Token *get_token_from_scanner(Scanner *scanner);
Token *get_identifier_or_keyword_from_scanner(Scanner *scanner);
Token *get_number_from_scanner(Scanner *scanner);
Token *get_operator_from_scanner(Scanner *scanner);
int is_operator(char a);
int is_keyword(char *a);
void scanner_exit_with_error(Scanner *scanner, char *error_msg);