#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../global.h"

#include "code_generator.h"
#include "../general/general.h"
#include "../compiler/compiler.h"
#include "../io/io.h"


void code_generator_create()
{
    compiler.code_generator = (Code_Generator*) calloc(1, sizeof(Code_Generator));
    if (compiler.code_generator == NULL) exit_memory_error(__FILE__, __LINE__);
}

void code_generator_destroy()
{
    if (compiler.code_generator != NULL)
    {
        // Close the destination file
        fclose(compiler.code_generator->dest_file);

        free(compiler.code_generator);
        compiler.code_generator = NULL;
    }
}

void code_generator_init(char* dest_file_name)
{
    // - Open destination file
    compiler.code_generator->dest_file = fopen(dest_file_name, "w");
    if (compiler.code_generator->dest_file == NULL) exit_file_io_error(dest_file_name);

    // - Init registers array
    int r = 0;

    strncpy(compiler.code_generator->registers[r].name, RBX, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R10, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R11, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R12, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R13, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R14, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;

    strncpy(compiler.code_generator->registers[r].name, R15, REGISTER_NAME_LENGTH);
    compiler.code_generator->registers[r++].inuse = false;
}

int code_generator_register_alloc()
{
    for (int r = 0; r < NUM_OF_REGISTERS; r++)
    {
        // If registes is not inuse, return its index
        if (!compiler.code_generator->registers[r].inuse)
        {
            compiler.code_generator->registers[r].inuse = true;
            return r;
        }
    }

    // TODO: Find better solution for not finding a free register
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
    sprintf(label, LABEL_FORMAT, label_num);

    // Increment static int for next label
    label_num++;

    // Return the new label
    return label;
}

char* code_generator_symbol_address(Symbol_Table_Entry* entry)
{
    if (entry == NULL)
        return NULL;

    // Allocate memory for the address
    char* address = (char*) calloc(SYMBOL_ADDRESS_LENGTH, sizeof(char));

    // - If the entry is global, just return a copy of the identifier
    if (entry->is_global)
    {
        sprintf(address, GLOBAL_ADDRESS_FORMAT, entry->identifier);
        return address;
    }

    // - If the entry is local, compute the appropriate address of that symbol

    // The offsest of the symbol is: 
    // ((number of entries seen until the parent scope) + (entry's order in it's symbol table)) * (the size of an entry in the stack)
    // For example:
    //      first symbol will be: 8 * (0 + 1) = 8 bytes -> [rbp - 8]
    //      second symbol will be: 8 * (0 + 2) = 16 bytes -> [rbp - 16]
    //      ...
    int bp_offset = STACK_ENTRY_BYTES * (entry->scope->parent->available_entries + entry->num_in_scope);

    // Create the computed stack address for the symbol
    sprintf(address, STACK_ADDRESS_FORMAT, bp_offset);

    // Return the computed address
    return address;
}

void code_generator_output(char* format, ...)
{
    va_list args;                                               // Declare a va_list type variable
    va_start(args, format);                                     // Initialize the va_list with the ...
    vfprintf(compiler.code_generator->dest_file, format, args); // Forward ... to vprintf
    va_end(args);                                               // Clean up the va_list
}

void code_generator_output_data_segment()
{
    Symbol_Table* global_table = compiler.scope_tree->global_scope->symbol_table;

    code_generator_output("\n.DATA\n");

    for (int i = 0; i < global_table->capacity; i++)
    {
        Symbol_Table_Entry* entry = global_table->entries[i];
        while (entry != NULL)
        {
            code_generator_output("\t" GLOBAL_ADDRESS_FORMAT, entry->identifier);
            code_generator_output("\tDQ ?\n"); // TODO: Add different data types
            entry = entry->next_entry;
        }
    }

    code_generator_output("\n");
}

void code_generator_generate(Parse_Tree_Node* parse_tree)
{
    // PROG -> prog id : BLOCK :)

    // Reset scope tree's scopes current_child_index to be STARTING_CHILD_INDEX before starting
    // the code generation process, so that the usage of the scope tree will be the same as the parsing phase.
    scope_tree_reset_child_index(compiler.scope_tree->global_scope);

    // - Output includes for needed libraries
    code_generator_output("includelib C:\\Ido_Hirsh\\Assembly\\masm64\\kernel32.lib\n");
    code_generator_output("includelib C:\\Ido_Hirsh\\Assembly\\masm64\\User32.lib\n\n");

    // - Extern the ExitProcess proc from kernel32.lib
    code_generator_output("EXTERN ExitProcess: PROC\n");

    // - Generate the data segment of the program
    code_generator_output_data_segment();

    // - Generate the code segment of the program
    code_generator_output(".CODE\n");
    code_generator_output("MAIN PROC\n");

    // Generate code for the main program block
    code_generator_block(parse_tree->children[3]);

    code_generator_output("CALL ExitProcess\n");
    code_generator_output("MAIN ENDP\n");
    code_generator_output("END\n");
}

void code_generator_block(Parse_Tree_Node* block)
{
    // BLOCK -> done
    if (block->num_of_children == 1)
        // Go to the parent scope when exiting a block
        scope_tree_goto_parent();

    // BLOCK -> STMT BLOCK
    else
    {
        code_generator_stmt(block->children[0]);
        code_generator_block(block->children[1]);
    }
}

void code_generator_stmt(Parse_Tree_Node* stmt)
{
    switch (stmt->children[0]->symbol)
    {
        case Non_Terminal_DECL:
            // STMT -> DECL
            code_generator_decl(stmt->children[0]);
            break;

        case Non_Terminal_ASSIGN:
            // STMT -> ASSIGN
            code_generator_assign(stmt->children[0]);
            break;

        case Non_Terminal_IF_ELSE:
            // STMT -> IF_ELSE
            code_generator_if_else(stmt->children[0]);
            break;

        case Non_Terminal_WHILE:
            // STMT -> WHILE
            code_generator_while(stmt->children[0]);
            break;
    }
}

void code_generator_decl(Parse_Tree_Node* decl)
{
    // DECL -> data_type id ;

    // If the current scope is not the global scope, add 1 to the scope's available entries
    // to help later calculate the address of the currently declared variable.
    // I don't want to count the global variables because they will be on the data segment
    // of the program and not the stack segment.
    if (compiler.scope_tree->current_scope != compiler.scope_tree->global_scope)
        compiler.scope_tree->current_scope->available_entries++;
}

void code_generator_assign(Parse_Tree_Node* assign)
{
    // ASSIGN -> set id = L_LOG_E ;

    // Calculate the expression
    code_generator_l_log_expr(assign->children[3]);

    // Assign the expression value to the variable

    // Free result register because we don't need it anymore

}

void code_generator_if_else(Parse_Tree_Node* if_else)
{
    // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE

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
        return;

    // ELSE -> else : BLOCK
    else
    {
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
        code_generator_h_log_expr(l_log_expr->children[0]);
    }

    // L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    else
    {
        code_generator_l_log_expr(l_log_expr->children[0]);
        code_generator_h_log_expr(l_log_expr->children[2]);
    }
}

void code_generator_h_log_expr(Parse_Tree_Node* h_log_expr)
{
    // H_LOG_E -> BOOL_E
    if (h_log_expr->num_of_children == 1)
    {
        code_generator_bool_expr(h_log_expr->children[0]);
    }

    // H_LOG_E -> H_LOG_E h_log_op BOOL_E
    else
    {
        code_generator_h_log_expr(h_log_expr->children[0]);
        code_generator_bool_expr(h_log_expr->children[2]);
    }
}

void code_generator_bool_expr(Parse_Tree_Node* bool_expr)
{
    // BOOL_E -> E
    if (bool_expr->num_of_children == 1)
    {
        code_generator_expr(bool_expr->children[0]);
    }

    // BOOL_E -> BOOL_E bool_op E
    else
    {
        code_generator_bool_expr(bool_expr->children[0]);
        code_generator_expr(bool_expr->children[2]);
    }
}

void code_generator_expr(Parse_Tree_Node* expr)
{
    // E -> T
    if (expr->num_of_children == 1)
    {
        code_generator_term(expr->children[0]);
    }

    // E -> E expr_op T
    else
    {
        code_generator_expr(expr->children[0]);
        code_generator_term(expr->children[2]);
    }
}

void code_generator_term(Parse_Tree_Node* term)
{
    // T -> F
    if (term->num_of_children == 1)
    {
        // Generate the factor
        code_generator_factor(term->children[0]);

        // Preserve the same register
        term->register_number = term->children[0]->register_number;
    }

    // T -> T term_op F
    else
    {
        // Generate the term
        code_generator_term(term->children[0]);

        // Generate the factor
        code_generator_factor(term->children[2]);

        // Perform the right operation between the term and the factor
        switch (term->children[1]->token->token_type)
        {
            case Token_Multiply:
                code_generator_T_mul_F(term);
                break;

            case Token_Divide:
                code_generator_T_div_F(term);
                break;

            case Token_Modulu:
                code_generator_T_modulu_F(term);
                break;
        }

        // The left operand gets the result. Preserve its register for the term
        term->register_number = term->children[0]->register_number;

        // Free the right register because we don't need it anymore
        code_generator_register_free(term->children[2]->register_number);
    }
}

void code_generator_T_mul_F(Parse_Tree_Node* term)
{
    code_generator_output(IMUL, code_generator_register_name(term->children[0]->register_number), code_generator_register_name(term->children[2]->register_number));
}

void code_generator_T_div_F(Parse_Tree_Node* term)
{
    code_generator_output(XOR, RDX, RDX);
    code_generator_output(MOV, RAX, code_generator_register_name(term->children[0]->register_number));
    code_generator_output(IDIV, code_generator_register_name(term->children[2]->register_number));
    code_generator_output(MOV, code_generator_register_name(term->children[0]->register_number), RAX);
}

void code_generator_T_modulu_F(Parse_Tree_Node* term)
{
    code_generator_output(XOR, RDX, RDX);
    code_generator_output(MOV, RAX, code_generator_register_name(term->children[0]->register_number));
    code_generator_output(IDIV, code_generator_register_name(term->children[2]->register_number));
    code_generator_output(MOV, code_generator_register_name(term->children[0]->register_number), RDX);
}

void code_generator_factor(Parse_Tree_Node* factor)
{
    switch (factor->children[0]->symbol)
    {
        case Token_Identifier:
            // F -> id
            code_generator_F_id(factor);
            break;

        case Token_Number:
        case Token_Character:
            // F -> literal
            code_generator_F_literal(factor);
            break;

        case Token_Open_Paren:
            // F -> ( L_LOG_E )
            code_generator_F_l_log_expr(factor);
            break;

        case Token_Not:
            // F -> ! F
            code_generator_F_not_F(factor);
            break;

        case Token_Minus:
            // F -> - F
            code_generator_F_minus_F(factor);
            break;
    }
}

void code_generator_F_id(Parse_Tree_Node* factor)
{
    // Allocate register
    factor->register_number = code_generator_register_alloc();

    // Calculate address for the given id
    char* address = code_generator_symbol_address(scope_tree_fetch(factor->children[0]->token->value));

    // Generate the code
    code_generator_output(MOV, code_generator_register_name(factor->register_number), address);

    // Free unneeded address
    free(address);
}

void code_generator_F_literal(Parse_Tree_Node* factor)
{
    // Allocate register
    factor->register_number = code_generator_register_alloc();

    // Generate the code
    code_generator_output(MOV, code_generator_register_name(factor->register_number), factor->children[0]->token->value);
}

void code_generator_F_l_log_expr(Parse_Tree_Node* factor)
{
    // Generate the expression
    code_generator_l_log_expr(factor->children[1]);

    // Preserve the expression's register
    factor->register_number = factor->children[1]->register_number;
}

void code_generator_F_not_F(Parse_Tree_Node* factor)
{
    // Generate the factor
    code_generator_factor(factor->children[1]);

    // Preserve the same register
    factor->register_number = factor->children[1]->register_number;

    // Generate the code
    char* false_label = code_generator_label_create();
    char* done_label = code_generator_label_create();
    code_generator_output(CMP, code_generator_register_name(factor->register_number), "0");
    code_generator_output(JE, false_label);
    code_generator_output(MOV, code_generator_register_name(factor->register_number), "0");
    code_generator_output(JMP, done_label);
    code_generator_output(LABEL, false_label);
    code_generator_output(MOV, code_generator_register_name(factor->register_number), "1");
    code_generator_output(LABEL, done_label);
}

void code_generator_F_minus_F(Parse_Tree_Node* factor)
{
    // Generate the factor
    code_generator_factor(factor->children[1]);

    // Preserve the same register
    factor->register_number = factor->children[1]->register_number;

    // Generate the code
    code_generator_output(NEG, code_generator_register_name(factor->register_number));
}
