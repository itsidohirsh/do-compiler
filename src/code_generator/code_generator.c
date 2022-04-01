#include <stdlib.h>
#include <stdio.h>

#include "../global.h"

#include "code_generator.h"
#include "../general/general.h"


void code_generator_create()
{
    compiler.code_generator = (Code_Generator*) calloc(1, sizeof(Code_Generator));
    if (compiler.code_generator == NULL) exit_memory_error(__FILE__, __LINE__);
}

void code_generator_destroy()
{
    if (compiler.code_generator != NULL)
    {
        free(compiler.code_generator);
        compiler.code_generator = NULL;
    }
}

void code_generator_init()
{
    // TODO: Add initialization for registers array
}

void code_generator_generate(Parse_Tree_Node* parse_tree)
{
    // PROG -> prog id : BLOCK :)
    printf("PROG -> prog id : BLOCK :)\n");
    // TODO: Clean scope tree's scopes current child index to be 0 before starting
    code_generator_block(parse_tree->children[3]);
}

void code_generator_block(Parse_Tree_Node* block)
{
    // BLOCK -> done
    if (block->num_of_children == 1)
        // TODO: Add go to parent in the scope tree
        printf("BLOCK -> done\n");

    // BLOCK -> STMT BLOCK
    else
    {
        printf("BLOCK -> STMT BLOCK\n");
        code_generator_stmt(block->children[0]);
        code_generator_block(block->children[1]);
    }
}

void code_generator_stmt(Parse_Tree_Node* stmt)
{
    switch (stmt->children[0]->symbol)
    {
        case Non_Terminal_DECL:
            printf("STMT -> DECL\n");
            code_generator_decl(stmt->children[0]);
            break;

        case Non_Terminal_ASSIGN:
            printf("STMT -> ASSIGN\n");
            code_generator_assign(stmt->children[0]);
            break;

        case Non_Terminal_IF_ELSE:
            printf("STMT -> IF_ELSE\n");
            code_generator_if_else(stmt->children[0]);
            break;

        case Non_Terminal_WHILE:
            printf("STMT -> WHILE\n");
            code_generator_while(stmt->children[0]);
            break;
    }
}

void code_generator_decl(Parse_Tree_Node* decl)
{
    // DECL -> data_type id ;
    printf("DECL -> data_type id ;\n");
    // TODO: Add variable from symbol table to data segment
}

void code_generator_assign(Parse_Tree_Node* assign)
{
    // ASSIGN -> set id = L_LOG_E ;
    printf("ASSIGN -> set id = L_LOG_E ;\n");
    code_generator_l_log_expr(assign->children[3]);
}

void code_generator_if_else(Parse_Tree_Node* if_else)
{
    printf("IF_ELSE\n");

    // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE
    code_generator_l_log_expr(if_else->children[2]);
    // TODO: Add goto child scope
    code_generator_block(if_else->children[5]);
    code_generator_else(if_else->children[6]);
}

void code_generator_else(Parse_Tree_Node* _else)
{
    printf("ELSE\n");

    // ELSE -> epsilon
    if (_else->num_of_children == 0)
        printf("ELSE -> epsilon\n");

    // ELSE -> else : BLOCK
    else
    {
        printf("ELSE -> else : BLOCK\n");
        // TODO: Add goto child scope
        code_generator_block(_else->children[2]);
    }
}

void code_generator_while(Parse_Tree_Node* _while)
{
    printf("WHILE\n");

    // WHILE -> while ( L_LOG_E ) : BLOCK
    code_generator_l_log_expr(_while->children[2]);
    // TODO: Add goto child scope
    code_generator_block(_while->children[5]);
}

void code_generator_l_log_expr(Parse_Tree_Node* l_log_expr)
{
    // L_LOG_E -> H_LOG_E
    if (l_log_expr->num_of_children == 1)
    {
        printf("L_LOG_E -> H_LOG_E\n");
        code_generator_h_log_expr(l_log_expr->children[0]);
    }

    // L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    else
    {
        printf("L_LOG_E -> L_LOG_E l_log_op H_LOG_E\n");
        code_generator_l_log_expr(l_log_expr->children[0]);
        code_generator_h_log_expr(l_log_expr->children[2]);
    }
}

void code_generator_h_log_expr(Parse_Tree_Node* h_log_expr)
{
    // H_LOG_E -> BOOL_E
    if (h_log_expr->num_of_children == 1)
    {
        printf("H_LOG_E -> BOOL_E\n");
        code_generator_bool_expr(h_log_expr->children[0]);
    }

    // H_LOG_E -> H_LOG_E h_log_op BOOL_E
    else
    {
        printf("H_LOG_E -> H_LOG_E h_log_op BOOL_E\n");
        code_generator_h_log_expr(h_log_expr->children[0]);
        code_generator_bool_expr(h_log_expr->children[2]);
    }
}

void code_generator_bool_expr(Parse_Tree_Node* bool_expr)
{
    // BOOL_E -> E
    if (bool_expr->num_of_children == 1)
    {
        printf("BOOL_E -> E\n");
        code_generator_expr(bool_expr->children[0]);
    }

    // BOOL_E -> BOOL_E bool_op E
    else
    {
        printf("BOOL_E -> BOOL_E bool_op E\n");
        code_generator_bool_expr(bool_expr->children[0]);
        code_generator_expr(bool_expr->children[2]);
    }
}

void code_generator_expr(Parse_Tree_Node* expr)
{
    // E -> T
    if (expr->num_of_children == 1)
    {
        printf("E -> T\n");
        code_generator_term(expr->children[0]);
    }

    // E -> E expr_op T
    else
    {
        printf("E -> E expr_op T\n");
        code_generator_expr(expr->children[0]);
        code_generator_term(expr->children[2]);
    }
}

void code_generator_term(Parse_Tree_Node* term)
{
    // T -> F
    if (term->num_of_children == 1)
    {
        printf("T -> F\n");
        code_generator_factor(term->children[0]);
    }

    // T -> T term_op F
    else
    {
        printf("T -> T term_op F\n");
        code_generator_term(term->children[0]);
        code_generator_factor(term->children[2]);
    }
}

void code_generator_factor(Parse_Tree_Node* factor)
{
    switch (factor->children[0]->symbol)
    {
        case Token_Identifier:
            printf("F -> id\n");
            break;

        case Token_Number:
        case Token_Character:
            printf("F -> literal\n");
            break;

        case Token_Open_Paren:
            printf("F -> ( L_LOG_E )\n");
            break;

        case Token_Not:
            printf("F -> ! F\n");
            break;

        case Token_Minus:
            printf("F -> - F\n");
            break;
    }
}
