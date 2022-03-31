#pragma once
#include <parser.h>

typedef struct {
    ParseTree *tree;
    char *code_string;
    int current; //current place in the code string
}
Generator;

Generator *new_generator(ParseTree *tree);
char *generate_declaration(ASTNode *node);
char *generate_expression(ASTNode *node);
