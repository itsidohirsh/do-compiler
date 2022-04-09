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

void code_generator_init()
{
    // - Open destination file
    compiler.code_generator->dest_file = fopen(compiler.dest_file_name, "w");
    if (compiler.code_generator->dest_file == NULL) exit_file_io_error(compiler.dest_file_name);

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

    // If couldn't find a free register
    printf(RED "Couldn't find a free register" RESET);
    compiler_destroy();
    remove(compiler.dest_file_name);
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
    code_generator_expression(assign->children[3]);

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
    code_generator_expression(if_else->children[2]);

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
    code_generator_expression(_while->children[2]);

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

void code_generator_expression(Parse_Tree_Node* expr)
{
    // Terminal
    if (expr->num_of_children == 0)
    {
        // Allocate register for the terminal
        expr->register_number = code_generator_register_alloc();

        // Move the terminal to the allocated register
        code_generator_mov_token(expr->register_number, expr->token);
    }

    // Unary expression
    // In the format: Expr -> operator Expr
    else if (expr->num_of_children == 2)
    {
        // Expr -> unary_operator Terminal
        if (expr->children[1]->symbol_type == Terminal)
        {
            // Allocate register for the terminal operand
            int register_number = code_generator_register_alloc();

            // Move the terminal to the allocated register
            code_generator_mov_token(register_number, expr->children[1]->token);

            // Generate code according to the unary operator
            code_generator_unary_expression(register_number, expr->children[0]->token->token_type);

            // Preserve the register
            expr->register_number = register_number;
        }

        // Expr -> unary_operator Non-Terminal
        else
        {
            // Generate the expression
            code_generator_expression(expr->children[1]);

            // Generate code according to the unary operator
            code_generator_unary_expression(expr->children[1]->register_number, expr->children[0]->token->token_type);

            // Preserve the register
            expr->register_number = expr->children[1]->register_number;
        }
    }

    // Binary expression
    // In the format: Expr -> Expr operator Expr
    else if (expr->num_of_children == 3)
    {
        // If both children are terminals
        // I want to start generating code only when both of the children are terminals,
        // that way I use the minimum number of registers to generate the target code, and minimizing
        // the risk of running out of registers.
        if (expr->children[0]->symbol_type == Terminal && expr->children[2]->symbol_type == Terminal)
        {
            // Allocate registers for the terminals
            int left_register = code_generator_register_alloc();
            int right_register = code_generator_register_alloc();

            // Move terminals to their registers
            code_generator_mov_token(left_register, expr->children[0]->token);
            code_generator_mov_token(right_register, expr->children[2]->token);

            // Generate code using both of the registers and the operator
            code_generator_binary_expression(left_register, right_register, expr->children[1]->token->token_type);

            // Preserve the left register for the result
            expr->register_number = left_register;

            // Free the right register because we don't need it anymore
            code_generator_register_free(right_register);
        }

        // If left is non-terminal and right is terminal
        else if (expr->children[0]->symbol_type == Non_Terminal && expr->children[2]->symbol_type == Terminal)
        {
            // Generate left expression
            code_generator_expression(expr->children[0]);

            // Allocate register for the right terminal
            int right_register = code_generator_register_alloc();

            // Move the right terminal to its register
            code_generator_mov_token(right_register, expr->children[2]->token);

            // Generate code using the left expression's register, right terminal and the operator
            code_generator_binary_expression(expr->children[0]->register_number, right_register, expr->children[1]->token->token_type);

            // Preserve the left register for the result
            expr->register_number = expr->children[0]->register_number;

            // Free the right register because we don't need it anymore
            code_generator_register_free(right_register);
        }

        // If left is terminal and right is non-terminal
        else if (expr->children[0]->symbol_type == Terminal && expr->children[2]->symbol_type == Non_Terminal)
        {
            // Generate the right expression
            code_generator_expression(expr->children[2]);

            // Allocate register for the left terminal
            int left_register = code_generator_register_alloc();

            // Move the left terminal to its register
            code_generator_mov_token(left_register, expr->children[0]->token);

            // Generate code using the right expression's register, left terminal and the operator
            code_generator_binary_expression(left_register, expr->children[2]->register_number, expr->children[1]->token->token_type);

            // Preserve the left register for the result
            expr->register_number = left_register;

            // Free the right register because we don't need it anymore
            code_generator_register_free(expr->children[2]->register_number);
        }

        // If both non-terminals
        else
        {
            // Generate the left and right expressions
            code_generator_expression(expr->children[0]);
            code_generator_expression(expr->children[2]);

            // Generate code using the left and the right expression's registers and the operator
            code_generator_binary_expression(expr->children[0]->register_number, expr->children[2]->register_number, expr->children[1]->token->token_type);

            // Preserve the left register for the result
            expr->register_number = expr->children[0]->register_number;

            // Free the right register because we don't need it anymore
            code_generator_register_free(expr->children[2]->register_number);
        }
    }
}

void code_generator_binary_expression(int left_register, int right_register, Token_Type operator)
{
    switch (operator)
    {
        case Token_Or:
            code_generator_or(left_register, right_register);
            break;

        case Token_And:
            code_generator_and(left_register, right_register);
            break;

        case Token_Equal:
        case Token_Not_Equal:
        case Token_Bigger:
        case Token_Bigger_Equal:
        case Token_Smaller:
        case Token_Smaller_Equal:
            code_generator_bool_e(left_register, right_register, operator);
            break;

        case Token_Plus:
            code_generator_output(ADD, code_generator_register_name(left_register), code_generator_register_name(right_register));
            break;

        case Token_Minus:
            code_generator_output(SUB, code_generator_register_name(left_register), code_generator_register_name(right_register));
            break;

        case Token_Multiply:
            code_generator_output(IMUL, code_generator_register_name(left_register), code_generator_register_name(right_register));
            break;

        case Token_Divide:
            code_generator_divide(left_register, right_register);
            break;

        case Token_Modulu:
            code_generator_modulu(left_register, right_register);
            break;
    }
}

void code_generator_unary_expression(int register_number, Token_Type operator)
{
    switch (operator)
    {
        case Token_Not:
            code_generator_not(register_number);
            break;

        case Token_Minus:
            code_generator_output(NEG, code_generator_register_name(register_number));
            break;
    }
}

void code_generator_bool_e(int left_register, int right_register, Token_Type operator)
{
    char* true_label = code_generator_label_create();
    char* done_label = code_generator_label_create();

    // Compare left and right operands
    code_generator_output(CMP, code_generator_register_name(left_register), code_generator_register_name(right_register));

    // Perform the right operation between the expr and the term
    switch (operator)
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
    code_generator_output(MOV, code_generator_register_name(left_register), "0");
    code_generator_output(JMP, done_label);

    // If jumped, condition was met and the result is 1
    code_generator_output(LABEL, true_label);
    code_generator_output(MOV, code_generator_register_name(left_register), "1");

    code_generator_output(LABEL, done_label);

    // Free the lables
    free(true_label);
    free(done_label);
}

void code_generator_mov_token(int register_number, Token* token)
{
    // Identifier
    if (token->token_type == Token_Identifier)
        code_generator_mov_identifier(register_number, token);

    // Literal
    else
        code_generator_output(MOV, code_generator_register_name(register_number), token->value);
}

void code_generator_mov_identifier(int register_number, Token* token)
{
    // Calculate address for the given id
    Symbol_Table_Entry* entry = scope_tree_fetch(token->value);

    // If the type of the identifier is char
    if (entry->data_type == Data_Type_Char)
    {
        // Zero out the register before moving a value to its lower bytes so the value will be correct
        code_generator_output(MOV, code_generator_register_name(register_number), "0");

        // Mov the value to the lower byte of the register
        code_generator_output("\tmov  %sb, %s\n", code_generator_register_name(register_number), code_generator_symbol_address(entry));
    }

    // If the type of the identifier is not char (int)
    else
        code_generator_output(MOV, code_generator_register_name(register_number), code_generator_symbol_address(entry));
}

void code_generator_or(int left_register, int right_register)
{
    char* true_label = code_generator_label_create();
    char* done_label = code_generator_label_create();

    // Check if left operand is not 0
    code_generator_output(NEG, code_generator_register_name(left_register));
    code_generator_output(JC, true_label);

    // Check if right operand is not 0
    code_generator_output(NEG, code_generator_register_name(right_register));
    code_generator_output(JC, true_label);

    // If both 0, result is 0
    code_generator_output(MOV, code_generator_register_name(left_register), "0");
    code_generator_output(JMP, done_label);

    // If either is not 0, result is 1
    code_generator_output(LABEL, true_label);
    code_generator_output(MOV, code_generator_register_name(left_register), "1");

    code_generator_output(LABEL, done_label);

    // Free the labels
    free(true_label);
    free(done_label);
}

void code_generator_and(int left_register, int right_register)
{
    char* false_label = code_generator_label_create();
    char* done_label = code_generator_label_create();

    // Check if left operand is 0
    code_generator_output(NEG, code_generator_register_name(left_register));
    code_generator_output(JNC, false_label);

    // Check if right operand is 0
    code_generator_output(NEG, code_generator_register_name(right_register));
    code_generator_output(JNC, false_label);

    // If both not 0, result is 1
    code_generator_output(MOV, code_generator_register_name(left_register), "1");
    code_generator_output(JMP, done_label);

    // If either is 0, result is 0
    code_generator_output(LABEL, false_label);
    code_generator_output(MOV, code_generator_register_name(left_register), "0");

    code_generator_output(LABEL, done_label);

    // Free the labels
    free(false_label);
    free(done_label);
}

void code_generator_divide(int left_register, int right_register)
{
    // Clear RDX, because the result of division is in RAX:RDX where RAX is quotient and RDX is remainder
    code_generator_output(MOV, RDX, "0");

    // Mov dividend to RAX for division
    code_generator_output(MOV, RAX, code_generator_register_name(left_register));

    // Divide by dividor
    code_generator_output(IDIV, code_generator_register_name(right_register));

    // Mov quotient to result register
    code_generator_output(MOV, code_generator_register_name(left_register), RAX);
}

void code_generator_modulu(int left_register, int right_register)
{
    // Clear RDX, because the result of division is in RAX:RDX where RAX is quotient and RDX is remainder
    code_generator_output(MOV, RDX, "0");

    // Mov dividend to RAX for division
    code_generator_output(MOV, RAX, code_generator_register_name(left_register));

    // Divide by dividor
    code_generator_output(IDIV, code_generator_register_name(right_register));

    // Mov remainder to result register
    code_generator_output(MOV, code_generator_register_name(left_register), RDX);
}

void code_generator_not(int register_number)
{
    // Initial result value is 1
    code_generator_output(MOV, RAX, "1");

    // NEG the factor register to know if 0 or not
    code_generator_output(NEG, code_generator_register_name(register_number));

    // Sub carry
    // If the number was not 0 CF = 1 and result will be 0
    // If the number was 0 CF = 0 and result will be 1
    code_generator_output(SBB, RAX, "0");

    // Mov result to result register
    code_generator_output(MOV, code_generator_register_name(register_number), RAX);
}
