#pragma once
#include "tokens.h"
#include "symbol-table.h"

typedef enum ASTNodeType
{
  AST_ROOT,
  AST_STMT,
  AST_FOR_LOOP,
  AST_EXPR,
  AST_ASSIGNMENT_DEST
} ASTNodeType;

typedef enum StatementType
{
  // var_type -> VariableType, var_name -> Name
  // Only allowed in the global scope
  STMT_DECL,
  // lhs -> AST_ASSIGNMENT_DEST, rhs->AST_EXPR
  STMT_ASSIGNMENT,
  STMT_PRINT_STMT
} StatementType;

typedef enum ExpressionType
{
  EXP_LITERAL,
  EXP_IDENT,
  EXP_BINOP,
  EXP_FUNC_CALL,
  EXP_LIST,
} ExpressionType;

typedef enum OperatorType
{
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_INDEX
} OperatorType;

typedef struct ForLoopClause
{
  char *ident;

  ASTNode *expr1;
  ASTNode *expr2;
  ASTNode *expr3;
} ForLoopClause;

typedef struct Assignment
{
  char *ident;
  ASTNode *expr;
} Assignment;

typedef struct ASTNode
{
  ASTNodeType type;

  StatementType stmt_type;

  // Declaration
  char *var_name;
  VariableType var_type;

  // Expression
  ExpressionType exp_type;

  OperatorType op_type;

  ASTNode *lhs;
  ASTNode *rhs;

  // Literal
  double value;

  // for loop, root node,
  ASTNode *contents;
  int num_contents;

  // For loop
  ForLoopClause *for_clause_1;
  // CAN BE NULL!
  ForLoopClause *for_clause_2;

  int line_number;
} ASTNode;

typedef struct ParseTree
{
  ASTNode *root;
  SymbolTable *symbol_table;
} ParseTree;

typedef struct Parser
{
  Token *tokens;
  int token_idx;
  ParseTree *parse_tree;
} Parser;