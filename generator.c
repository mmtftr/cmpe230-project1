#include <generator.h>
#include <defs.h>
#include <string.h>

Generator *new_generator(ParseTree *tree)
{
    Generator *generator = calloc(1, sizeof(Generator));
    generator->tree = tree;
    generator->code_string = calloc(1, LINE_LIMIT*LINE_LIMIT);
    char *string = new_code_string();
    strcpy(generator->code_string, string);
    generator->current = strlen(string);
}

char *new_code_string(void)
{
    return "#include <stdio.h>\n\
            void main()\n\
            {\n\
            ";
}

char *generate_declaration(ASTNode *node)
{
    char *string = calloc(1, LINE_LIMIT);
    if (node->var_type.var_type == TYPE_SCALAR)
    {
        strcat(string, "\tdouble ");
        strcat(string, node->var_name);
        strcat(string, " ;\n");
    }
    else if (node->var_type.var_type == TYPE_VECTOR)
    {
        strcat(string, "\tdouble ");
        strcat(string, node->var_name);
        strcat(string, "[");
        sprintf(string, "%d", node->var_type.height);
        strcat(string, "];\n");
    }
    else if (node->var_type.var_type == TYPE_MATRIX)
    {
        strcat(string, "\tdouble ");
        strcat(string, node->var_name);
        strcat(string, "[");
        sprintf(string, "%d", node->var_type.height);
        strcat(string, "]");
        strcat(string, "[");
        sprintf(string, "%d", node->var_type.width);
        strcat(string, "];\n");
    }
    return string;
}

char *generate_expression(ASTNode *node)
{
    char *string = calloc(1, LINE_LIMIT);
    // node is an AST_EXPR
    if (node->exp_type == EXP_IDENT)
    {
        strcpy(string, node->ident);
    }
}

char *generate_assignment(ASTNode *node)
{
    char *string = calloc(1, LINE_LIMIT);
    if (node->lhs->)
}

void traverse_and_generate(ParseTree *tree)
{

}

void dfs(ASTNode *node, Generator *generator)
{
    if (node->type == AST_ROOT)
    {
        dfs(node->contents, generator);
    }
    else if (node->type == AST_STMT)
    {
        if (node->stmt_type == STMT_DECL)
        {
            char *string = generate_declaration(node);
            strcopy(&generator->code_string[generator->current], string);
            generator->current += strlen(string);
        }
        else if (node->stmt_type == STMT_ASSIGNMENT)
        {
            dfs(node->lhs, generator);
        }
    }
}