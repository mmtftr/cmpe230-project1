#include <string.h>
#include "defs.h"
#include "tokens.h"
#include "parser.h"
#include "symbol-table.h"

ASTNode *new_ast_node(ASTNodeType type, int line_number)
{
  ASTNode *node = calloc(1, sizeof(ASTNode));

  node->type = type;
  node->line_number = line_number;

  return node;
}

ParseTree *new_parse_tree()
{
  ParseTree *tree = malloc(sizeof(ParseTree));
  tree->root = new_ast_node(AST_ROOT, 1);
  tree->symbol_table = new_symbol_table();
  return tree;
}

Parser *new_parser(Token *tokenList)
{
  Parser *parser = calloc(1, sizeof(Parser));
  parser->tokens = tokenList;
  parser->token_idx = 0;
  parser->parse_tree = new_parse_tree();
}

Token *get_curr(Parser *parser)
{
  return &parser->tokens[parser->token_idx];
}

int is_eof(Parser *parser)
{
  return (get_curr(parser)->type == TKN_EOF);
}

Token *advance(Parser *parser)
{
  if (is_eof(parser))
  {
    return NULL;
  }
  parser->token_idx++;
  return peek_prev(parser);
}

Token *go_back(Parser *parser)
{
  if (parser->token_idx == 0)
  {
    return NULL;
  }
  parser->token_idx--;
  return peek_next(parser);
}

Token *peek_next(Parser *parser)
{
  if (is_eof(parser))
  {
    return NULL;
  }
  return &parser->tokens[parser->token_idx + 1];
}

Token *peek_prev(Parser *parser)
{
  if (parser->token_idx == 0)
  {
    return NULL;
  }
  return &parser->tokens[parser->token_idx - 1];
}

int match(Parser *parser, TokenType type)
{
  if (get_curr(parser)->type == type)
  {
    advance(parser);
    return 1;
  }
  return 0;
}

ASTNode *parse_root(Parser *parser, ASTNode *root)
{
  root->contents = calloc(256, sizeof(ASTNode));
  root->num_contents = 0;

  while (!is_eof(parser))
  {
    if (match(parser, TKN_KW_FOR))
    {
      ASTNode *node = new_ast_node(AST_FOR_LOOP, get_curr(parser)->line_num);
      node->contents = calloc(LINE_LIMIT, sizeof(ASTNode));
      parse_for_loop_statement(parser, node);
      continue;
    }
    ASTNode *node = parse_statement(parser);
    if (node != NULL)
    {
      add_child(root, node);
    }
  }

  return root;
}

ASTNode *parse_statement(Parser *parser)
{
  if (match(parser, TKN_IDENT))
  {
    go_back(parser);
    return parse_assignment(parser);
  }
  else if (match(parser, TKN_TYPE_MATRIX) || match(parser, TKN_TYPE_SCALAR) || match(parser, TKN_TYPE_VECTOR))
  {
    go_back(parser);
    return parse_declaration(parser);
  }
}

ASTNode *parse_assignment(Parser *parser)
{
  ASTNode *node = new_ast_node(AST_STMT, get_curr(parser)->line_num);

  node->stmt_type = STMT_ASSIGNMENT;
  ASTNode *lhs = parse_assignment_dest(parser);
  if (match(parser, TKN_ASSIGN))
  {
    ASTNode *rhs = parse_expression(parser);
    node->rhs = rhs;
    node->lhs = lhs;
  }
  else
  {
    exit_with_error(parser, "Expected '=' after identifier");
  }

  return node;
}

ASTNode *parse_declaration(Parser *parser)
{
  ASTNode *node = new_ast_node(AST_STMT, get_curr(parser)->line_num);
  node->stmt_type = STMT_DECL;

  VariableType type = get_var_type(get_curr(parser)->type);
  if (!match(parser, TKN_IDENT))
  {
    exit_with_error(parser, "Expected identifier after type name");
  }
  char *ident = get_curr(parser)->contents;

  node->var_name = ident;
  node->var_type = type;

  Variable var = {.name = ident, .type = type, .height = 1, .width = 1};
  if (type == TYPE_VECTOR)
  {
    if (!match(parser, TKN_PN_OPENBRACKET))
    {
      exit_with_error(parser, "Expected '[' after vector declaration");
    }
    if (!match(parser, TKN_INT_LITERAL))
    {
      exit_with_error(parser, "Expected integer after '['");
    }

    var.height = atoi(get_curr(parser)->contents);

    if (!match(parser, TKN_PN_CLOSEBRACKET))
    {
      exit_with_error(parser, "Expected ']' after vector size");
    }
  }
  else if (type == TYPE_MATRIX)
  {
    if (!match(parser, TKN_PN_OPENBRACKET))
    {
      exit_with_error(parser, "Expected '[' after matrix declaration");
    }
    if (!match(parser, TKN_INT_LITERAL))
    {
      exit_with_error(parser, "Expected integer after '['");
    }

    var.height = atoi(get_curr(parser)->contents);

    if (!match(parser, TKN_PN_COMMA))
    {
      exit_with_error(parser, "Expected ',' after matrix width");
    }
    if (!match(parser, TKN_INT_LITERAL))
    {
      exit_with_error(parser, "Expected integer after ','");
    }

    var.width = atoi(get_curr(parser)->contents);

    if (!match(parser, TKN_PN_CLOSEBRACKET))
    {
      exit_with_error(parser, "Expected ']' after matrix size");
    }
  }

  add_variable(parser->parse_tree->symbol_table, var);
  return node;
}

void add_child(ASTNode *parent, ASTNode *child)
{
  parent->contents[parent->num_contents] = child;
  parent->num_contents++;
}

void exit_with_error(Parser *parser, char *message)
{
  printf("Error: (Line %d) %s\n", message, get_curr(parser)->line_num);
  exit(1);
}