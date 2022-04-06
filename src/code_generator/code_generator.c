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

    // Static string for the address (to prevent allocation each time we need a symbol)
    static char address[SYMBOL_ADDRESS_LENGTH] = { 0 };

    // - If the entry is global, just return the identifier
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
    if (entry->data_type == Data_Type_Char)
        sprintf(address, "byte ptr " STACK_ADDRESS_FORMAT, bp_offset);
    else
        sprintf(address, "qword ptr " STACK_ADDRESS_FORMAT, bp_offset);

    // Return the computed address
    return address;
}

void code_generator_output(char* format, ...)
{
    va_list args;                                               // Declare a va_list type variable
    va_start(args, format);                                     // Initialize the va_list with the ...
    vfprintf(compiler.code_generator->dest_file, format, args); // Forward ... to vfprintf
    va_end(args);                                               // Clean up the va_list
}

void code_generator_output_data_segment()
{
    Symbol_Table* global_table = compiler.scope_tree->global_scope->symbol_table;

    code_generator_output(".data\n");

    for (int i = 0; i < global_table->capacity; i++)
    {
        Symbol_Table_Entry* entry = global_table->entries[i];
        while (entry != NULL)
        {
            code_generator_output("\t" GLOBAL_ADDRESS_FORMAT, entry->identifier);
            code_generator_output(entry->data_type == Data_Type_Char ? DB : DQ);
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

    // - Generate the data segment of the program
    code_generator_output_data_segment();

    // - Generate the code segment of the program
    code_generator_output(".code\n");
    code_generator_output("main proc\n");
    code_generator_output("\tpush rbp\n");
    code_generator_output("\tmov  rbp, rsp\n");
    code_generator_output("\tsub  rsp, 1000h\n\n");

    // Generate code for the main program block
    code_generator_block(parse_tree->children[3]);

    code_generator_output("\n\tmov  rsp, rbp\n");
    code_generator_output("\tpop  rbp\n");
    code_generator_output("\tmov  rax, 0\n");
    code_generator_output("\tret\n");
    code_generator_output("main endp\n");
    code_generator_output("end\n");
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
        // Generate the statement
        code_generator_stmt(block->children[0]);

        // Generate the block
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
    Symbol_Table_Entry* entry = scope_tree_fetch(assign->children[1]->token->value);

    if (entry->data_type == Data_Type_Char)
        code_generator_output("\tmov  %s, %sb\n", code_generator_symbol_address(entry), code_generator_register_name(assign->children[3]->register_number));
    else
        code_generator_output(MOV, code_generator_symbol_address(entry), code_generator_register_name(assign->children[3]->register_number));

    // Free result register because we don't need it anymore
    code_generator_register_free(assign->children[3]->register_number);
}

void code_generator_if_else(Parse_Tree_Node* if_else)
{
    // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE

    // Go to next children when entering a new scope
    scope_tree_goto_child();

    // Update the child to have the same number of available entries as the current scope
    compiler.scope_tree->current_scope->available_entries = compiler.scope_tree->current_scope->parent->available_entries;

    char* else_label = code_generator_label_create();
    char* done_label = code_generator_label_create();

    // Generate the expression
    code_generator_l_log_expr(if_else->children[2]);

    // Check if condition
    code_generator_output(CMP, code_generator_register_name(if_else->children[2]->register_number), "0");
    code_generator_output(JE, else_label);

    // Free expression register because we don't need it anymore
    code_generator_register_free(if_else->children[2]->register_number);

    // Generate if block
    code_generator_block(if_else->children[5]);
    code_generator_output(JMP, done_label);

    // Generate else block
    code_generator_output(LABEL, else_label);
    code_generator_else(if_else->children[6]);

    code_generator_output(LABEL, done_label);

    // Free labels
    free(else_label);
    free(done_label);
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

        // Generate else block
        code_generator_block(_else->children[2]);
    }
}

void code_generator_while(Parse_Tree_Node* _while)
{
    // WHILE -> while ( L_LOG_E ) : BLOCK

    // Go to next children when entering a new scope
    scope_tree_goto_child();

    // Update the child to have the same number of available entries as the current scope
    compiler.scope_tree->current_scope->available_entries = compiler.scope_tree->current_scope->parent->available_entries;

    char* while_label = code_generator_label_create();
    char* done_label = code_generator_label_create();

    code_generator_output(LABEL, while_label);

    // Generate the expression
    code_generator_l_log_expr(_while->children[2]);

    // Check while condition
    code_generator_output(CMP, code_generator_register_name(_while->children[2]->register_number), "0");
    code_generator_output(JE, done_label);

    // Free expression register because we don't need it anymore
    code_generator_register_free(_while->children[2]->register_number);

    // Generate while block
    code_generator_block(_while->children[5]);
    code_generator_output(JMP, while_label);

    code_generator_output(LABEL, done_label);

    // Free labels
    free(while_label);
    free(done_label);
}

void code_generator_l_log_expr(Parse_Tree_Node* l_log_expr)
{
    // L_LOG_E -> H_LOG_E
    if (l_log_expr->num_of_children == 1)
    {
        // Generate the h_log_expr
        code_generator_h_log_expr(l_log_expr->children[0]);

        // Preserve the same register
        l_log_expr->register_number = l_log_expr->children[0]->register_number;
    }

    // L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    else
    {
        // Generate the l_log_expr
        code_generator_l_log_expr(l_log_expr->children[0]);

        // Generate the h_log_expr
        code_generator_h_log_expr(l_log_expr->children[2]);

        char* true_label = code_generator_label_create();
        char* done_label = code_generator_label_create();

        // Check if left operand is not 0
        code_generator_output(NEG, code_generator_register_name(l_log_expr->children[0]->register_number));
        code_generator_output(JC, true_label);

        // Check if right operand is not 0
        code_generator_output(NEG, code_generator_register_name(l_log_expr->children[2]->register_number));
        code_generator_output(JC, true_label);

        // If both 0, result is 0
        code_generator_output(MOV, code_generator_register_name(l_log_expr->children[0]->register_number), "0");
        code_generator_output(JMP, done_label);

        // If either is not 0, result is 1
        code_generator_output(LABEL, true_label);
        code_generator_output(MOV, code_generator_register_name(l_log_expr->children[0]->register_number), "1");

        code_generator_output(LABEL, done_label);

        // Free the lables
        free(true_label);
        free(done_label);

        // The left operand gets the result. Preserve its register for the l_log_expr
        l_log_expr->register_number = l_log_expr->children[0]->register_number;

        // Free the right register because we don't need it anymore
        code_generator_register_free(l_log_expr->children[2]->register_number);
    }
}

void code_generator_h_log_expr(Parse_Tree_Node* h_log_expr)
{
    // H_LOG_E -> BOOL_E
    if (h_log_expr->num_of_children == 1)
    {
        // Generate the bool_expr
        code_generator_bool_expr(h_log_expr->children[0]);

        // Preserve the same register
        h_log_expr->register_number = h_log_expr->children[0]->register_number;
    }

    // H_LOG_E -> H_LOG_E h_log_op BOOL_E
    else
    {
        // Generate the h_log_expr
        code_generator_h_log_expr(h_log_expr->children[0]);

        // Generate the bool_expr
        code_generator_bool_expr(h_log_expr->children[2]);

        char* false_label = code_generator_label_create();
        char* done_label = code_generator_label_create();

        // Check if left operand is 0
        code_generator_output(NEG, code_generator_register_name(h_log_expr->children[0]->register_number));
        code_generator_output(JNC, false_label);

        // Check if right operand is 0
        code_generator_output(NEG, code_generator_register_name(h_log_expr->children[2]->register_number));
        code_generator_output(JNC, false_label);

        // If both not 0, result is 1
        code_generator_output(MOV, code_generator_register_name(h_log_expr->children[0]->register_number), "1");
        code_generator_output(JMP, done_label);

        // If either is 0, result is 0
        code_generator_output(LABEL, false_label);
        code_generator_output(MOV, code_generator_register_name(h_log_expr->children[0]->register_number), "0");

        code_generator_output(LABEL, done_label);

        // Free the lables
        free(false_label);
        free(done_label);

        // The left operand gets the result. Preserve its register for the h_log_expr
        h_log_expr->register_number = h_log_expr->children[0]->register_number;

        // Free the right register because we don't need it anymore
        code_generator_register_free(h_log_expr->children[2]->register_number);
    }
}

void code_generator_bool_expr(Parse_Tree_Node* bool_expr)
{
    // BOOL_E -> E
    if (bool_expr->num_of_children == 1)
    {
        // Generate the expr
        code_generator_expr(bool_expr->children[0]);

        // Preserve the same register
        bool_expr->register_number = bool_expr->children[0]->register_number;
    }

    // BOOL_E -> BOOL_E bool_op E
    else
    {
        // Generate the bool_expr
        code_generator_bool_expr(bool_expr->children[0]);

        // Generate the expr
        code_generator_expr(bool_expr->children[2]);

        char* true_label = code_generator_label_create();
        char* done_label = code_generator_label_create();

        // Compare left and right operands
        code_generator_output(CMP, code_generator_register_name(bool_expr->children[0]->register_number), code_generator_register_name(bool_expr->children[2]->register_number));

        // Perform the right operation between the expr and the term
        switch (bool_expr->children[1]->symbol)
        {
            case Token_Equal:
                code_generator_output(JE, true_label);
                break;

            case Token_Not_Equal:
                code_generator_output(JNE, true_label);
                break;

            case Token_Bigger:
                code_generator_output(JG, true_label);
                break;

            case Token_Bigger_Equal:
                code_generator_output(JGE, true_label);
                break;

            case Token_Smaller:
                code_generator_output(JL, true_label);
                break;

            case Token_Smaller_Equal:
                code_generator_output(JLE, true_label);
                break;
        }

        // If didn't jump, condition was not met and the result is 0
        code_generator_output(MOV, code_generator_register_name(bool_expr->children[0]->register_number), "0");
        code_generator_output(JMP, done_label);

        // If jumped, condition was met and the result is 1
        code_generator_output(LABEL, true_label);
        code_generator_output(MOV, code_generator_register_name(bool_expr->children[0]->register_number), "1");

        code_generator_output(LABEL, done_label);

        // Free the lables
        free(true_label);
        free(done_label);

        // The left operand gets the result. Preserve its register for the bool_expr
        bool_expr->register_number = bool_expr->children[0]->register_number;

        // Free the right register because we don't need it anymore
        code_generator_register_free(bool_expr->children[2]->register_number);
    }
}

void code_generator_expr(Parse_Tree_Node* expr)
{
    // E -> T
    if (expr->num_of_children == 1)
    {
        // Generate the term
        code_generator_term(expr->children[0]);

        // Preserve the same register
        expr->register_number = expr->children[0]->register_number;
    }

    // E -> E expr_op T
    else
    {
        // Generate expr
        code_generator_expr(expr->children[0]);

        // Generate term
        code_generator_term(expr->children[2]);

        // Perform the right operation between the expr and the term
        switch (expr->children[1]->symbol)
        {
            case Token_Plus:
                code_generator_output(ADD, code_generator_register_name(expr->children[0]->register_number), code_generator_register_name(expr->children[2]->register_number));
                break;

            case Token_Minus:
                code_generator_output(SUB, code_generator_register_name(expr->children[0]->register_number), code_generator_register_name(expr->children[2]->register_number));
                break;
        }

        // The left operand gets the result. Preserve its register for the expr
        expr->register_number = expr->children[0]->register_number;

        // Free the right register because we don't need it anymore
        code_generator_register_free(expr->children[2]->register_number);
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
        switch (term->children[1]->symbol)
        {
            case Token_Multiply:
                code_generator_output(IMUL, code_generator_register_name(term->children[0]->register_number), code_generator_register_name(term->children[2]->register_number));
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

void code_generator_T_div_F(Parse_Tree_Node* term)
{
    // Clear RDX, because the result of division is in RAX:RDX where RAX is quotient and RDX is remainder
    code_generator_output(MOV, RDX, "0");

    // Mov dividend to RAX for division
    code_generator_output(MOV, RAX, code_generator_register_name(term->children[0]->register_number));

    // Divide by dividor
    code_generator_output(IDIV, code_generator_register_name(term->children[2]->register_number));

    // Mov quotient to result register
    code_generator_output(MOV, code_generator_register_name(term->children[0]->register_number), RAX);
}

void code_generator_T_modulu_F(Parse_Tree_Node* term)
{
    // Clear RDX, because the result of division is in RAX:RDX where RAX is quotient and RDX is remainder
    code_generator_output(MOV, RDX, "0");

    // Mov dividend to RAX for division
    code_generator_output(MOV, RAX, code_generator_register_name(term->children[0]->register_number));

    // Divide by dividor
    code_generator_output(IDIV, code_generator_register_name(term->children[2]->register_number));

    // Mov remainder to result register
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
    Symbol_Table_Entry* entry = scope_tree_fetch(factor->children[0]->token->value);

    if (entry->data_type == Data_Type_Char)
    {
        // Zero out the register before moving a value to its lower bytes so the value will be correct
        code_generator_output(MOV, code_generator_register_name(factor->register_number), "0");

        // Mov the value to the lower byte of the register
        code_generator_output("\tmov  %sb, %s\n", code_generator_register_name(factor->register_number), code_generator_symbol_address(entry));
    }
    else
        code_generator_output(MOV, code_generator_register_name(factor->register_number), code_generator_symbol_address(entry));
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

    // Initial result value is 1
    code_generator_output(MOV, RAX, "1");

    // NEG the factor register to know if 0 or not
    code_generator_output(NEG, code_generator_register_name(factor->children[1]->register_number));

    // Sub carry
    // If the number was not 0 CF = 1 and result will be 0
    // If the number was 0 CF = 0 and result will be 1
    code_generator_output(SBB, RAX, "0");

    // Mov result to result register
    code_generator_output(MOV, code_generator_register_name(factor->children[1]->register_number), RAX);
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
