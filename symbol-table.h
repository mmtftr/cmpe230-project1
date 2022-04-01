#pragma once
typedef enum VariableType //TypeName
{
  TYPE_SCALAR,
  TYPE_VECTOR,
  TYPE_MATRIX,
}
VariableType;

typedef struct ResultType //DataType
{
  VariableType var_type;
  // For scalars both = 1, for vectors width = 1 and height = length of vector
  int height;
  int width;
}
ResultType;

typedef struct Variable
{
  ResultType type; //DataType
  char *name;
}
Variable;

typedef struct SymbolTable
{
  Variable *variables;
  int num_variables;
}
SymbolTable;


SymbolTable *new_symbol_table();
Variable *lookup_variable(SymbolTable *table, char *name);
void add_variable(SymbolTable *table, Variable var);
VariableType get_var_type(TokenType type);