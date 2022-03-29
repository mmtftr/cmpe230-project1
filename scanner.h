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
static char advance_chr(Scanner *scanner);
static void go_back_chr(Scanner *scanner);
static char peek_chr(Scanner *scanner);
static char peek_next_chr(Scanner *scanner);
static char peek_prev_chr(Scanner *scanner);
static int match_chr(Scanner *scanner, char c);
Token *get_tokens_from_file(char *filename);
Scanner *scan_str(char *str);
Token *get_tokens_with_scanner(Scanner *scanner);
Token *get_token_from_scanner(Scanner *scanner);
Token *get_identifier_or_keyword_from_scanner(Scanner *scanner);
Token *get_number_from_scanner(Scanner *scanner);
Token *get_operator_from_scanner(Scanner *scanner);
int is_operator(char a);
int is_keyword(char *a);
static void scanner_exit_with_error(Scanner *scanner, char *error_msg);