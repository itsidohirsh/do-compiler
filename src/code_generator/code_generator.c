#include <stdlib.h>
#include <string.h>

#include "../global.h"

#include "code_generator.h"
#include "../general/general.h"
#include "../compiler/compiler.h"


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
    int r = 0;
    // rbx
    strncpy(compiler.code_generator->registers[r].name, "rbx", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r10
    strncpy(compiler.code_generator->registers[r].name, "r10", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r11
    strncpy(compiler.code_generator->registers[r].name, "r11", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r12
    strncpy(compiler.code_generator->registers[r].name, "r12", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r13
    strncpy(compiler.code_generator->registers[r].name, "r13", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r14
    strncpy(compiler.code_generator->registers[r].name, "r14", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
    // r15
    strncpy(compiler.code_generator->registers[r].name, "r15", REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
}

int code_generator_register_alloc()
{
    int r;
    for (r = 0; r < NUM_OF_REGISTERS; r++)
    {
        // If registers is no inuse, return its index
        if (!compiler.code_generator->registers[r].inuse)
        {
            compiler.code_generator->registers[r].inuse = true;
            return r;
        }
    }

    // TODO: Find better solution than that
    // If couldn't find a free register, output error and exit
    printf(RED "Couldn't find a free register" RESET);
    compiler_destroy();
    exit(1);
}

void code_generator_register_free(int r)
{
    compiler.code_generator->registers[r].inuse = false;
}

char* code_generator_register_name(int r)
{
    return compiler.code_generator->registers[r].name;
}

char* code_generator_label_create()
{
    // A static integer to create unique lables
    static int label_num = 0;

    // Allocate memory for the label
    char* label = (char*) calloc(LABEL_NAME_LENGTH, sizeof(char));
    if (label == NULL) exit_memory_error(__FILE__, __LINE__);

    // Create label
    sprintf(label, ".L%d", label_num);

    // Increment static int for next label
    label_num++;

    // Return the new label
    return label;
}

char* code_generator_symbol_address(Symbol_Table_Entry* entry)
{
    if (entry == NULL)
        return NULL;

    // - If the entry is global, just return a copy of the identifier
    if (entry->is_global)
        return strdup(entry->identifier);

    // - If the entry is local, compute the appropriate address of that symbol

    // Allocate memory for the address
    char* address = (char*) calloc(SYMBOL_ADDRESS_LENGTH, sizeof(char));

    // The offsest of the symbol is: 
    // ((number of entries seen until the parent scope) + (entry's order in it's symbol table)) * (the size of an entry in the stack)
    // For example:
    //      first symbol will be: 8 * (0 + 1) = 8 bytes -> [bp - 8]
    //      second symbol will be: 8 * (0 + 2) = 16 bytes -> [bp - 16]
    //      ...
    int bp_offset = STACK_ENTRY_BYTES * (entry->scope->parent->available_entries + entry->num_in_scope);

    // Create the computed stack address for the symbol
    sprintf(address, "[bp - %d]", bp_offset);

    // Return the computed address
    return address;
}

void code_generator_generate(Parse_Tree_Node* parse_tree)
{
    // PROG -> prog id : BLOCK :)
    printf("PROG -> prog id : BLOCK :)\n");

    // Reset scope tree's scopes current_child_index to be STARTING_CHILD_INDEX before starting
    scope_tree_reset_child_index(compiler.scope_tree->global_scope);

    // Generate code for the main program block
    code_generator_block(parse_tree->children[3]);
}

void code_generator_block(Parse_Tree_Node* block)
{
    // BLOCK -> done
    if (block->num_of_children == 1)
    {
        printf("BLOCK -> done\n");

        // Go to the parent scope when exiting a block
        scope_tree_goto_parent();
    }

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

    // If the current scope is not the global scope, add 1 to the scope's available entries.
    // I don't want to cound the global variables because they will be on the data segment
    // and not on the stack
    if (compiler.scope_tree->current_scope != compiler.scope_tree->global_scope)
        compiler.scope_tree->current_scope->available_entries++;
}

void code_generator_assign(Parse_Tree_Node* assign)
{
    // ASSIGN -> set id = L_LOG_E ;
    printf("ASSIGN -> set id = L_LOG_E ;\n");

    code_generator_l_log_expr(assign->children[3]);
}

void code_generator_if_else(Parse_Tree_Node* if_else)
{
    // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE
    printf("IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE\n");

    code_generator_l_log_expr(if_else->children[2]);

    // Go to next children when entering a new scope
    scope_tree_goto_child();
    // Update the child to have the same number of available entries as the current scope
    compiler.scope_tree->current_scope->available_entries = compiler.scope_tree->current_scope->parent->available_entries;

    code_generator_block(if_else->children[5]);
    code_generator_else(if_else->children[6]);
}

void code_generator_else(Parse_Tree_Node* _else)
{
    // ELSE -> epsilon
    if (_else->num_of_children == 0)
        printf("ELSE -> epsilon\n");

    // ELSE -> else : BLOCK
    else
    {
        printf("ELSE -> else : BLOCK\n");

        // Go to next children when entering a new scope
        scope_tree_goto_child();
        // Update the child to have the same number of available entries as the current scope
        compiler.scope_tree->current_scope->available_entries = compiler.scope_tree->current_scope->parent->available_entries;

        code_generator_block(_else->children[2]);
    }
}

void code_generator_while(Parse_Tree_Node* _while)
{
    // WHILE -> while ( L_LOG_E ) : BLOCK
    printf("WHILE -> while ( L_LOG_E ) : BLOCK\n");

    code_generator_l_log_expr(_while->children[2]);

    // Go to next children when entering a new scope
    scope_tree_goto_child();
    // Update the child to have the same number of available entries as the current scope
    compiler.scope_tree->current_scope->available_entries = compiler.scope_tree->current_scope->parent->available_entries;

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
            printf("F -> number literal\n");
            break;

        case Token_Character:
            printf("F -> character literal\n");
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
