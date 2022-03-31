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

  assert(cts[0].type == AST_STMT);
  assert(cts[0].stmt_type == STMT_DECL);
  assert(cts[0].var_type.var_type == TYPE_MATRIX);
  assert(cts[0].var_type.height == 4);
  assert(cts[0].var_type.width == 4);
  assert(strcmp(cts[0].var_name, "A") == 0);

  assert(cts[1].type == AST_STMT);
  assert(cts[1].stmt_type == STMT_DECL);
  assert(cts[1].var_type.var_type == TYPE_MATRIX);
  assert(cts[1].var_type.height == 1);
  assert(cts[1].var_type.width == 1);
  assert(strcmp(cts[1].var_name, "T") == 0);

  assert(cts[2].type == AST_STMT);
  assert(cts[2].stmt_type == STMT_DECL);
  assert(cts[2].var_type.var_type == TYPE_VECTOR);
  assert(cts[2].var_type.height == 4);
  assert(cts[2].var_type.width == 1);
  assert(strcmp(cts[2].var_name, "x") == 0);

  assert(cts[3].type == AST_STMT);
  assert(cts[3].stmt_type == STMT_DECL);
  assert(cts[3].var_type.var_type == TYPE_VECTOR);
  assert(cts[3].var_type.height == 4);
  assert(cts[3].var_type.width == 1);
  assert(strcmp(cts[3].var_name, "xy2") == 0);

  assert(cts[4].type == AST_STMT);
  assert(cts[4].stmt_type == STMT_DECL);
  assert(cts[4].var_type.var_type == TYPE_SCALAR);
  assert(strcmp(cts[4].var_name, "s") == 0);

  assert(cts[5].type == AST_STMT);
  assert(cts[5].stmt_type == STMT_ASSIGNMENT);
  assert(cts[5].lhs->type == AST_ASSIGNMENT_DEST);
  assert(cts[5].rhs->type == AST_EXPR);
  assert(cts[5].rhs->exp_type == EXP_LIST);

  ASTNode *contents = cts[5].rhs->contents;
  assert(contents[0].exp_type == EXP_LITERAL);
  assert(contents[0].literal_value == 0);
  assert(contents[1].exp_type == EXP_LITERAL);
  assert(contents[1].literal_value == 1);
  assert(contents[2].exp_type == EXP_LITERAL);
  assert(contents[2].literal_value == 2);
  assert(contents[3].exp_type == EXP_LITERAL);
  assert(contents[3].literal_value == 3);
  assert(contents[4].exp_type == EXP_LITERAL);
  assert(contents[4].literal_value == 4);
  assert(contents[5].exp_type == EXP_LITERAL);
  assert(contents[5].literal_value == 5);
  assert(contents[6].exp_type == EXP_LITERAL);
  assert(contents[6].literal_value == 6);
  assert(contents[7].exp_type == EXP_LITERAL);
  assert(contents[7].literal_value == 7);
  assert(contents[8].exp_type == EXP_LITERAL);
  assert(contents[8].literal_value == 8);
  assert(contents[9].exp_type == EXP_LITERAL);
  assert(contents[9].literal_value == 9);
  assert(contents[10].exp_type == EXP_LITERAL);
  assert(contents[10].literal_value == 1);
  assert(contents[11].exp_type == EXP_LITERAL);
  assert(contents[11].literal_value == 1);
  assert(contents[12].exp_type == EXP_LITERAL);
  assert(contents[12].literal_value == 1);
  assert(contents[13].exp_type == EXP_LITERAL);
  assert(contents[13].literal_value == 2);
  assert(contents[14].exp_type == EXP_LITERAL);
  assert(contents[14].literal_value == 3);
  assert(contents[15].exp_type == EXP_LITERAL);
  assert(contents[15].literal_value == 4);

  assert(cts[6].type == AST_STMT);
  assert(cts[6].stmt_type == STMT_ASSIGNMENT);
  assert(cts[6].lhs->type == AST_ASSIGNMENT_DEST);
  assert(cts[6].rhs->type == AST_EXPR);
  assert(cts[6].rhs->exp_type == EXP_LIST);

  contents = cts[6].rhs->contents;
  assert(contents[0].exp_type == EXP_LITERAL);
  assert(contents[0].literal_value == 1);
  assert(contents[1].exp_type == EXP_LITERAL);
  assert(contents[1].literal_value == 1);
  assert(contents[2].exp_type == EXP_LITERAL);
  assert(contents[2].literal_value == 1);
  assert(contents[3].exp_type == EXP_LITERAL);
  assert(contents[3].literal_value == 1);

  assert(cts[8].type == AST_STMT);
  assert(cts[8].stmt_type == STMT_PRINT_STMT);
  assert(cts[8].contents->type == AST_EXPR);
}

int is_expr_with_type(ASTNode *node, ExpressionType exp_type)
{
  if (node->type != AST_EXPR)
  {
    return 0;
  }
  return node->exp_type == exp_type;
}