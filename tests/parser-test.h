#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "exit-stub.h"
#include "../tokens.h"
#include "../scanner.h"
#include "../parser.h"

void run_parser_tests();
void parse_challenge_1();
void parse_challenge_2();
void parse_challenge_3();
void parse_challenge_4();
void parse_challenge_5();
void parse_fail_challenge1();
int is_expr_with_type(ASTNode *node, ExpressionType exp_type);
int is_literal_with_val(ASTNode *node, char *cts);
int is_expr_with_result_type(ASTNode *node, ResultType type);
int is_decl_with_type_and_name(ASTNode *node, ResultType type, char *name);
int is_list_assignment(ASTNode *node, char *destination_ident, char **expected_contents);

void run_parser_tests()
{
  printf("Running parser tests...\n");
  reset_exit_stub();
  parse_challenge_1();
  reset_exit_stub();
  parse_challenge_2();
  reset_exit_stub();
  parse_challenge_3();
  reset_exit_stub();
  parse_challenge_4();
  reset_exit_stub();
  parse_challenge_5();
  reset_exit_stub();
  parse_fail_challenge1();
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

  is_decl_with_type_and_name(cts, (ResultType){.var_type = TYPE_SCALAR, .height = 1, .width = 1}, "a");
}

void parse_challenge_2()
{
  char *challenge_1 = "matrix _a[1,2]";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  is_decl_with_type_and_name(cts, (ResultType){.var_type = TYPE_MATRIX, .height = 1, .width = 2}, "_a");
}

void parse_challenge_3()
{
  char *challenge_1 = "vector _a_2[3]";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  is_decl_with_type_and_name(cts, (ResultType){.var_type = TYPE_VECTOR, .height = 3, .width = 1}, "_a_2");
}

void parse_challenge_4()
{
  char *challenge_1 = "matrix A[3,2]\n\
  A=   {        0.5 0 0.50 0 0.5 0.510}";

  ASTNode *root = parse_return_root(challenge_1);
  ASTNode *cts = root->contents;

  assert(is_decl_with_type_and_name(&cts[0], (ResultType){.var_type = TYPE_MATRIX, .width = 2, .height = 3}, "A"));

  assert(cts[1].type == AST_STMT);
  assert(cts[1].stmt_type == STMT_ASSIGNMENT);
  assert(cts[1].lhs->type == AST_ASSIGNMENT_DEST);
  assert(cts[1].rhs->type == AST_EXPR);
  assert(cts[1].rhs->exp_type == EXP_LIST);

  ASTNode *contents = cts[1].rhs->contents;
  char *expected_contents[] = {"0.5", "0", "0.50", "0", "0.5", "0.510"};
  assert(cts[1].rhs->num_contents == 6);
  for (int i = 0; i < cts[1].rhs->num_contents; i++)
  {
    assert(is_literal_with_val(&contents[i], expected_contents[i]));
  }
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

  assert(is_decl_with_type_and_name(&cts[0], (ResultType){.var_type = TYPE_MATRIX, .width = 4, .height = 4}, "A"));

  assert(is_decl_with_type_and_name(&cts[1], (ResultType){.var_type = TYPE_MATRIX, .width = 1, .height = 1}, "T"));

  assert(is_decl_with_type_and_name(&cts[2], (ResultType){.var_type = TYPE_VECTOR, .width = 1, .height = 4}, "x"));

  assert(is_decl_with_type_and_name(&cts[3], (ResultType){.var_type = TYPE_VECTOR, .width = 1, .height = 4}, "xy2"));

  assert(is_decl_with_type_and_name(&cts[4], (ResultType){.var_type = TYPE_SCALAR, .height = 1, .width = 1}, "s"));

  char **expected_contents = (char *[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "1", "1", "1", "2", "3", "4"};
  assert(is_list_assignment(&cts[5], "A", expected_contents));

  expected_contents = (char *[]){"1", "1", "1", "1"};
  assert(is_list_assignment(&cts[6], "x", expected_contents));

  assert(cts[8].type == AST_STMT);
  assert(cts[8].stmt_type == STMT_PRINT_STMT);
  assert(is_expr_with_result_type(cts[8].contents, (ResultType){.var_type = TYPE_SCALAR, .height = 1, .width = 1}));
}

void parse_fail_challenge1()
{
  char *challenge_1 = "matrix T[1,2]\n\
  matrix A[1,2]\n\
  A = { 2 3 }\n\
  T = tr(A)";

  should_exit = 1;
  done = 0;
  expected_code = 1;
  int return_val = 0;
  if (!(return_val = setjmp(jump_env)))
  {
    parse_return_root(challenge_1);
  }
  assert(return_val == 1);
}

int is_expr_with_type(ASTNode *node, ExpressionType exp_type)
{
  if (node->type != AST_EXPR)
  {
    return 0;
  }
  return node->exp_type == exp_type;
}

int is_literal_with_val(ASTNode *node, char *cts)
{
  return is_expr_with_type(node, EXP_LITERAL) && strcmp(node->literal_str, cts) == 0;
}

int is_expr_with_result_type(ASTNode *node, ResultType type)
{
  return node->type == AST_EXPR && node->exp_result_type.var_type == type.var_type && node->exp_result_type.width == type.width && node->exp_result_type.height == type.height;
}
int is_decl_with_type_and_name(ASTNode *node, ResultType type, char *name)
{
  return node->stmt_type == STMT_DECL && node->var_type.var_type == type.var_type && node->var_type.width == type.width && node->var_type.height == type.height && strcmp(node->var_name, name) == 0;
}
int is_list_assignment(ASTNode *node, char *destination_ident, char **expected_contents)
{
  int is_list_type = node->stmt_type == STMT_ASSIGNMENT && node->lhs->type == AST_ASSIGNMENT_DEST && node->rhs->type == AST_EXPR && node->rhs->exp_type == EXP_LIST;
  if (!is_list_type)
    return 0;
  if (strcmp(node->lhs->var_name, destination_ident) != 0)
    return 0;
  if (node->lhs->var_type.var_type == TYPE_SCALAR)
  {
    // This should never happen if we are type-checking
    printf("Error: List assignment to scalar\n");
    exit(1);
  }
  ASTNode *contents = node->rhs->contents;
  for (int i = 0; i < node->rhs->num_contents; i++)
  {
    if (!is_literal_with_val(&contents[i], expected_contents[i]))
      return 0;
  }
  return 1;
}