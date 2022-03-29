#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "defs.h"
#include "scanner.h"

void scanner_init(Scanner *scanner, char *contents, int line_num)
{
  scanner->contents = contents;
  scanner->line_num = line_num;
  scanner->pos = 0;
  scanner->len = strlen(contents);
}

char advance(Scanner *scanner)
{
  scanner->pos++;
  if (scanner->pos - 1 >= scanner->len)
  {
    return '\0';
  }
  char c = scanner->contents[scanner->pos - 1];
  return c;
}

void go_back(Scanner *scanner)
{
  if (scanner->pos == 0)
  {
    return;
  }
  scanner->pos--;
}

char peek(Scanner *scanner)
{
  if (scanner->pos >= scanner->len)
  {
    return '\0';
  }
  return scanner->contents[scanner->pos];
}

char peek_next(Scanner *scanner)
{
  if (scanner->pos + 1 >= scanner->len)
  {
    return '\0';
  }
  return scanner->contents[scanner->pos + 1];
}

char peek_prev(Scanner *scanner)
{
  if (scanner->pos - 1 < 0)
  {
    return '\0';
  }
  return scanner->contents[scanner->pos - 1];
}

int match(Scanner *scanner, char c)
{
  if (scanner->pos >= scanner->len)
  {
    return 0;
  }
  if (scanner->contents[scanner->pos] == c)
  {
    scanner->pos++;
    return 1;
  }
  return 0;
}

Token *get_tokens_from_file(char *filename)
{
  FILE *fp;
  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Cannot open %s\n", filename);

    return NULL;
  }
  char line[256];
  char contents[LINE_LIMIT * LINE_LIMIT];

  int line_num = 1;
  while (fgets(line, LINE_LIMIT, fp) != NULL)
  {
    strcat(contents, line);
    strcat(contents, "\n");
    line_num++;
  }
  fclose(fp);

  Scanner *scanner = scan_str(contents);
  Token *tokens = scanner->scanned_tokens;
  free(scanner);
  return tokens;
}

// Scan tokens from a string.
// Returns a scanner with null if there is an error.
Scanner *scan_str(char *str)
{
  Scanner *scanner = calloc(1, sizeof(Scanner));
  scanner_init(scanner, str, 1);
  scanner->scanned_tokens = get_tokens_with_scanner(scanner);

  return scanner;
}

Token *get_tokens_with_scanner(Scanner *scanner)
{
  Token *tokens = calloc(LINE_LIMIT * LINE_LIMIT, sizeof(Token));
  int tokens_index = 0;

  while (scanner->pos < scanner->len)
  {
    Token *token = get_token_from_scanner(scanner);
    if (token->type == TKN_LINE_FEED)
    {
      scanner->line_num++;
    }
    tokens[tokens_index] = *token;
    free(token);
    tokens_index++;
  }

  return tokens;
}

Token *get_token_from_scanner(Scanner *scanner)
{
  char c = advance(scanner);
  while (c)
  {
    switch (c)
    {
    case ' ':
      break;
    case '\n':
      return new_token(TKN_LINE_FEED, strdup("\n"), scanner->line_num);
    case ',':
      return new_token(TKN_PN_COMMA, strdup(","), scanner->line_num);
    case '(':
      return new_token(TKN_PN_OPENPAREN, strdup("("), scanner->line_num);
    case ')':
      return new_token(TKN_PN_CLOSEPAREN, strdup(")"), scanner->line_num);
    case '{':
      return new_token(TKN_PN_OPENBRACE, strdup("{"), scanner->line_num);
    case '}':
      return new_token(TKN_PN_CLOSEBRACE, strdup("}"), scanner->line_num);
    case '[':
      return new_token(TKN_PN_OPENBRACKET, strdup("["), scanner->line_num);
    case ']':
      return new_token(TKN_PN_CLOSEBRACKET, strdup("]"), scanner->line_num);
    case ':':
      return new_token(TKN_PN_COLON, strdup(":"), scanner->line_num);
    case '=':
      return new_token(TKN_ASSIGN, strdup("="), scanner->line_num);
    default:
      if (isalpha(c))
      {
        return get_identifier_or_keyword_from_scanner(scanner);
      }
      else if (isdigit(c))
      {
        return get_number_from_scanner(scanner);
      }
      else if (is_operator(c))
      {
        return get_operator_from_scanner(scanner);
      }
      else
      {
        scanner_exit_with_error(scanner, "Unknown character");
      }
    }

    // In case we got whitespace, keep on scanning
    c = advance(scanner);
  }
  if (c == '\0')
  {
    return new_token(TKN_EOF, strdup(""), scanner->line_num);
  }
}

Token *get_identifier_or_keyword_from_scanner(Scanner *scanner)
{
  char ident[LINE_LIMIT];

  char c = peek_prev(scanner);
  int i = 0;

  while (isalnum(c))
  {
    ident[i] = c;

    i++;
    c = advance(scanner);
  }
  go_back(scanner);
  ident[i] = '\0';

  if (is_keyword(ident))
  {
    TokenType type;
    if (strcmp(ident, "scalar") == 0)
    {
      type = TKN_TYPE_SCALAR;
    }
    if (strcmp(ident, "vector") == 0)
    {
      type = TKN_TYPE_VECTOR;
    }
    if (strcmp(ident, "matrix") == 0)
    {
      type = TKN_TYPE_MATRIX;
    }
    if (strcmp(ident, "for") == 0)
    {
      type = TKN_KW_FOR;
    }
    if (strcmp(ident, "in") == 0)
    {
      type = TKN_KW_IN;
    }
    if (strcmp(ident, "print") == 0)
    {
      type = TKN_SPECIAL_PRINT;
    }
    if (strcmp(ident, "printsep") == 0)
    {
      type = TKN_SPECIAL_PRINTSEP;
    }
    if (strcmp(ident, "tr") == 0)
    {
      type = TKN_FUNCTION_TR;
    }
    if (strcmp(ident, "choose") == 0)
    {
      type = TKN_FUNCTION_CHOOSE;
    }
    if (strcmp(ident, "sqrt") == 0)
    {
      type = TKN_FUNCTION_SQRT;
    }
    return new_token(type, strdup(ident), scanner->line_num);
  }
  return new_token(TKN_IDENT, strdup(ident), scanner->line_num);
}

Token *get_number_from_scanner(Scanner *scanner)
{
  char number[LINE_LIMIT];

  char c = peek_prev(scanner);
  int i = 0;

  int is_float = 0;

  while (isdigit(c))
  {
    number[i] = c;

    i++;
    c = advance(scanner);
  }
  if (c == '.')
  {
    is_float = 1;
    number[i] = c;
    i++;
    c = advance(scanner);
    if (!isdigit(c))
    {
      scanner_exit_with_error(scanner, "Expected a digit after .");
    }
    while (isdigit(c))
    {
      number[i] = c;
      i++;
      c = advance(scanner);
    }
  }
  go_back(scanner);

  number[i] = '\0';
  return new_token(is_float ? TKN_FLOAT_LITERAL : TKN_INT_LITERAL, strdup(number), scanner->line_num);
}

Token *get_operator_from_scanner(Scanner *scanner)
{
  char c = peek_prev(scanner);
  TokenType type;
  switch (c)
  {
  case '*':
    type = TKN_MULT;
    break;
  case '+':
    type = TKN_PLUS;
    break;
  case '-':
    type = TKN_MINUS;
    break;
  default:
    return new_token(TKN_UNKNOWN, NULL, scanner->line_num);
  }
  char op[2] = {c, '\0'};
  return new_token(type, strdup(op), scanner->line_num);
}

int is_operator(char a)
{
  switch (a)
  {
  case '*':
  case '+':
  case '-':
    return 1;
  default:
    return 0;
  }
}

static const char **KEYWORDS = {"for", "in", "scalar", "vector", "matrix", "print", "printsep"};

int is_keyword(char *a)
{
  for (int i = 0; i < sizeof(KEYWORDS) / sizeof(char *); i++)
  {
    if (strcmp(a, KEYWORDS[i]) == 0)
    {
      return 1;
    }
  }
  return 0;
}

void scanner_exit_with_error(Scanner *scanner, char *error_msg)
{
  printf("Error (Line %d): %s\n", scanner->line_num, error_msg);
}