#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../tokens.h"
#include "../scanner.h"
#include "../parser.h"

void run_parser_tests();
void parse_challenge_1();
void parse_challenge_2();
void parse_challenge_3();
void parse_challenge_4();
void parse_challenge_5();

void run_parser_tests()
{
  printf("Running parser tests...\n");
  parse_challenge_1();
  parse_challenge_2();
  parse_challenge_3();
  parse_challenge_4();
  parse_challenge_5();
  printf("Parser tests passed!\n");
}

ASTNode *parse_return_root(char *input)
{
  Token *tokenList = scan_str(input)->scanned_tokens;
  Parser *parser = new_parser(tokenList);
  ASTNode *root = new_ast_node(AST_ROOT, 0);
  root = parse_root(parser, root);
  return root;
}

void parse_challenge_1()
{
  char *challenge_1 = "scalar a";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  assert(cts[0].type == AST_STMT);
  assert(cts[0].stmt_type == STMT_DECL);
  assert(cts[0].var_type.var_type == TYPE_SCALAR);
  assert(strcmp(cts[0].var_name, "a") == 0);
}

void parse_challenge_2()
{
  char *challenge_1 = "matrix _a[1,2]";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  assert(cts[0].type == AST_STMT);
  assert(cts[0].stmt_type == STMT_DECL);
  assert(cts[0].var_type.var_type == TYPE_MATRIX);
  assert(cts[0].var_type.height == 1);
  assert(cts[0].var_type.width == 2);
  assert(strcmp(cts[0].var_name, "_a") == 0);
}

void parse_challenge_3()
{
  char *challenge_1 = "vector _a_2[3]";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  assert(cts[0].type == AST_STMT);
  assert(cts[0].stmt_type == STMT_DECL);
  assert(cts[0].var_type.var_type == TYPE_VECTOR);
  assert(cts[0].var_type.height == 3);
  assert(cts[0].var_type.width == 1);
  assert(strcmp(cts[0].var_name, "_a_2") == 0);
}

void parse_challenge_4()
{
  char *challenge_1 = "matrix A[3,2]\n\
  A=   {        0.5 0 0.50 0 0.5 0.510}";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  assert(cts[0].type == AST_STMT);
  assert(cts[0].stmt_type == STMT_DECL);
  assert(cts[0].var_type.var_type == TYPE_MATRIX);
  assert(cts[0].var_type.height == 3);
  assert(cts[0].var_type.width == 2);
  assert(strcmp(cts[0].var_name, "A") == 0);

  assert(cts[1].type == AST_STMT);
  assert(cts[1].stmt_type == STMT_ASSIGNMENT);
  assert(cts[1].lhs->type == AST_ASSIGNMENT_DEST);
  assert(cts[1].rhs->type == AST_EXPR);
  assert(cts[1].rhs->exp_type == EXP_LIST);

  ASTNode *contents = cts[1].rhs->contents;
  assert(contents[0].exp_type == EXP_LITERAL);
  assert(contents[0].literal_value == 0.5);
  assert(contents[1].exp_type == EXP_LITERAL);
  assert(contents[1].literal_value == 0);
  assert(contents[2].exp_type == EXP_LITERAL);
  assert(contents[2].literal_value == 0.5);
  assert(contents[3].exp_type == EXP_LITERAL);
  assert(contents[3].literal_value == 0);
  assert(contents[4].exp_type == EXP_LITERAL);
  assert(contents[4].literal_value == 0.5);
  assert(contents[5].exp_type == EXP_LITERAL);
  assert(contents[5].literal_value == 0.51);
}

void parse_challenge_5()
{
  char *challenge_1 = "matrix A[4,4]\n\
matrix T[1,1]\n\
vector x[4]\n\
 vector xy2[4]\n\
  scalar s\n\
A = {0 1 2 3 4 5 6 7 8 9 1 1 1 2 3 4 }\n\
 x = {1 1 1 1 }\n\
xy2 = { 2 1 3 1 }\n\
print(s)\n\
 T = tr(x)*A*xy2\n\
 s = T[1,1]";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;
}