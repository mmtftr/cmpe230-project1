#pragma once
enum VariableType
{
  TYPE_SCALAR,
  TYPE_VECTOR,
  TYPE_MATRIX,
};

typedef struct ResultType
{
  VariableType var_type;
  // For scalars both = 1, for vectors width = 1 and height = length of vector
  int height;
  int width;
} ResultType;

typedef enum VariableType VariableType;

struct Variable
{
  ResultType type;
  char *name;
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