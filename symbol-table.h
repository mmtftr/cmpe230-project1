#pragma once
enum VariableType
{
  TYPE_SCALAR,
  TYPE_VECTOR,
  TYPE_MATRIX,
};

typedef enum VariableType VariableType;

struct Variable
{
  VariableType type;
  char *name;
  int height;
  int width;
};

typedef struct Variable Variable;

struct SymbolTable
{
  Variable *variables;
  int num_variables;
};
typedef struct SymbolTable SymbolTable;

SymbolTable *new_symbol_table();
Variable *lookup_variable(SymbolTable *table, char *name);
void add_variable(SymbolTable *table, Variable var);
VariableType get_var_type(TokenType type);