#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "tokens.h"
#include "symbol-table.h"

SymbolTable *new_symbol_table()
{
  SymbolTable *table = calloc(1, sizeof(SymbolTable));
  table->variables = calloc(LINE_LIMIT, sizeof(Variable));
  table->num_variables = 0;
  return table;
}

void add_variable(SymbolTable *table, Variable var)
{
  table->variables[table->num_variables] = var;
  table->num_variables++;
}

Variable *lookup_variable(SymbolTable *table, char *name)
{
  for (int i = 0; i < table->num_variables; i++)
  {
    if (strcmp(table->variables[i].name, name) == 0)
    {
      return &table->variables[i];
    }
  }
  return NULL;
}

VariableType get_var_type(TokenType type)
{
  switch (type)
  {
  case TKN_TYPE_SCALAR:
    return TYPE_SCALAR;
  case TKN_TYPE_VECTOR:
    return TYPE_VECTOR;
  case TKN_TYPE_MATRIX:
    return TYPE_MATRIX;
  default:
    return 0;
  }
}