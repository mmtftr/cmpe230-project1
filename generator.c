#include <generator.h>
#include <defs.h>
#include <string.h>

Generator *new_generator(ParseTree *tree)
{
    Generator *generator = calloc(1, sizeof(Generator));
    generator->tree = tree;
    generator->code_string = calloc(1, 10*LINE_LIMIT*LINE_LIMIT); //not sure
    char *string = new_code_string();
    strcpy(generator->code_string, string);
    generator->current = strlen(string);
}

char *new_code_string()
{//not sure
    return strcat(preamble, "#include <stdio.h>\n\n\
            void main()\n\
            {\n\
            ");
}

void generate_statement(Generator *generator, ASTNode *node)
{
    switch (node->stmt_type)
    {
        case STMT_DECL:
            generate_declaration_stmt(node, generator);
            break;
        case STMT_ASSIGNMENT:
            generate_assignment_stmt(node, generator);
            break;
        case STMT_PRINT_STMT:
            generate_print_stmt(node, generator);
            break;
    }
}

void generate_declaration_stmt(Generator *generator, ASTNode *node)
{
    if (node->var_type.var_type == TYPE_SCALAR)
    {
        generate_str("double ", generator);
        generate_str(node->var_name, generator);
        generate_chr(';', generator);
    }
    else if (node->var_type.var_type == TYPE_VECTOR)
    {
        generate_str("double **", generator);
        generate_str(node->var_name, generator);
        generate_chr('[', generator);
        char format[LINE_LIMIT];
        sprintf(format, "%d", node->var_type.height);
        generate_str(format, generator);
        generate_str("];", generator);
    }
    else if (node->var_type.var_type == TYPE_MATRIX)
    {
        generate_str("double **", generator);
        generate_str(node->var_name, generator);
        generate_chr('[', generator);
        char format_1[LINE_LIMIT];
        sprintf(format_1, "%d", node->var_type.height);
        gnerate_str(format_1, generator);
        generate_chr(']', generator);
        generate_chr('[', generator);
        char format_2[LINE_LIMIT];
        sprintf(format_2, "%d", node->var_type.width);
        generate_str(format_2, generator);
        generate_str("];", generator);
    }
}

void generate_assignment_stmt(Generator *generator, ASTNode *node)
{
    generate_assignment_dest(node->lhs, generator);
    generate_str(" = ", generator);
    generate_expression(node->rhs, generator);
    generate_chrr(';', generator);

}

void generate_assignment_dest(Generator *generator, ASTNode *node)
{

}

void generate_print_stmt(Generator *generator, ASTNode *node)
{

}

void generate_expression(Generator *generator, ASTNode *node)
{
    switch (node->exp_type)
    {
        case EXP_LITERAL:
            break;
        case EXP_LIST:
            break;
        case EXP_IDENT:
            break;
        case EXP_BINOP:
            break;
        case EXP_FUNC_CALL:
            break;
        case EXP_INDEX:
            break;
    }
    // node is an AST_EXPR
    if (node->exp_type == EXP_IDENT)
    {
        char format[LINE_LIMIT];
        sprintf(format, "%d", node->literal_value); //not sure
        generate_str(format, generator);
        
    }
    else if (node->exp_type == EXP_LITERAL)
    {
        char format[LINE_LIMIT];
        sprintf(format, "%d", node->literal_value); //not sure
        generate_str(format, generator);
    }
    else if (node->exp_type == EXP_IDENT)
    {
        generate_identifier(node, generator);
    }
    else if (node->exp_type == EXP_BINOP)
    {
        if (node->op_type == OP_MULT)
        {
            generate_chr(generator, '*');
        }
        else if (node->op_type == OP_PLUS)
        {
            generate_chr(generator, '+');
        }
        else if (node->op_type == OP_MINUS)
        {
            generate_chr(generator, '-');
        }
    }
    else if (node->exp_type == EXP_INDEX)
    {
        generate_identifier(node->contents, generator);
        generate_left_bracket(generator);
    }
}

void generate_for_loop(Generator *generator, ASTNode *node)
{
    generate_chr(generator, '\t');
    generate_for_header(generator, node);
    generate_chr(generator, '\n');
    for (int i = 0 ; i < node->num_contents ; i++)
    {
        generate_str(generator, "\t\t");
        generate_statement(generator, node);
        generate_chr(generator, '\n');
    }
}

void generate_for_header(Generator *generator, ASTNode *node)
{

}

void generate_chr(Generator *generator, char chr)
{
    char append[2] = {chr, '\0'};
    strcat(generator->code_string, append);
}

void generate_str(Generator *generator, char *string)
{
    strcat(generator->code_string, string);
}

void generate_identifier(Generator *generator, ASTNode *node)
{
    strcat(generator->code_string, node->ident);
}

void generate_code_string(Generator *generator, ParseTree *tree)
{
    for (int i = 0 ; i < tree->root->num_contents ; ++i)
    {
        traverse_and_generate(&tree->root->contents[i], generator);
    }
}

void traverse_and_generate(Generator *generator, ASTNode *node)
{
    switch (node->type)
    {
        case AST_STMT:
            generate_chr(generator, '\t');
            generate_statement(generator, node);
            generate_chr(generator, '\n');
        case AST_FOR_LOOP:
            generate_chr(generator, '\t');
            generate_for_loop(generator, node);
    }       
    for (int i = 0 ; i < node->num_contents ; ++i)
    {
        traverse_and_generate(&node->contents[i], generator);
    }
}