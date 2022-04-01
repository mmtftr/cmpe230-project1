#pragma once
#include <parser.h>

typedef struct {
    ParseTree *tree;
    char *code_string;
    int current; //current place in the code string
}
Generator;

extern char *preamble;
Generator *new_generator(ParseTree *tree);

