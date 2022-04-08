#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "code_generator_base.h"
#include "../parser/parse_tree/parse_tree.h"


/* ---------- Structs ---------- */

// Struct of a register in the code generator's register array
typedef struct Register
{
    char name[REGISTER_NAME_LENGTH];        // The name of the register for target code output
    bool inuse;                             // Whether the register is currently in use or not
} Register;

// Struct of the code generator
typedef struct Code_Generator
{
    Register registers[NUM_OF_REGISTERS];   // Array of registers to be used in the code generation process
    FILE* dest_file;                        // A pointer to the output file for the generated code
} Code_Generator;


/* ---------- Functions ---------- */

// Create a new code generator and points the compiler's code generator to it
void code_generator_create();

// Frees the memory allocated for the code generator of the compiler
void code_generator_destroy();

// Gets the name of the destination file, and initializes the compiler's code generator
void code_generator_init(char* dest_file_name);

// Searches for a free register in the code generator registers array.
// If found, marks it as inuse and returns the index of that register.
// If not found, terminates the compiler.
int code_generator_register_alloc();

// Marks the register in index r in the code generator registers array as unused
void code_generator_register_free(int r);

// Returns the register name of register r in the code generator registers array
char* code_generator_register_name(int r);

// Allocates a new lable name and returns a pointer to it
char* code_generator_label_create();

// Performs the right address computation for the given symbol according to its
// place in the program (global / local),
// and returns a string that represents that address.
// If the entry is NULL, returns NULL.
char* code_generator_symbol_address(Symbol_Table_Entry* entry);

// Outputs the given formated string to the target file
void code_generator_output(char* format, ...);

// Outputs the data segment of the program to the target file
void code_generator_output_data_segment();

// Generates the assembly code for the given parse tree
void code_generator_generate(Parse_Tree_Node* parse_tree);

// Generates a block (BLOCK)
void code_generator_block(Parse_Tree_Node* block);

// Generates a statement (STMT)
void code_generator_stmt(Parse_Tree_Node* stmt);

// Generates a variable declaration statement (DECL)
void code_generator_decl(Parse_Tree_Node* decl);

// Generates an assignment statement (ASSIGN)
void code_generator_assign(Parse_Tree_Node* assign);

// Generates an if else statement (IF_ELSE)
void code_generator_if_else(Parse_Tree_Node* if_else);

// Generates an else statement (ELSE)
void code_generator_else(Parse_Tree_Node* _else);

// Generates a while statement (WHILE)
void code_generator_while(Parse_Tree_Node* _while);

// Generates an expression
void code_generator_expression(Parse_Tree_Node* expr);

// Generates a binary expression of the form: Expr -> Expr binary_operator Expr
void code_generator_binary_expression(int left_register, int right_register, Token_Type operator);

// Generates an unary expression of the form: Expr -> unary_operator Expr
void code_generator_unary_expression(int register_number, Token_Type operator);

// Generates a boolean expression of the form: Expr -> bool_operator Expr
// according to the given operator
void code_generator_bool_e(int left_register, int right_register, Token_Type operator);

// Generates target code that moves the given token to the given register.
// Moves the token according to its type (identifier / literal)
void code_generator_mov_token(int register_number, Token* token);

// Generates target code that moves the given identifier token to the given register.
void code_generator_mov_identifier(int register_number, Token* token);

// Generates logical OR and puts the result (0 | 1) in the left register
void code_generator_or(int left_register, int right_register);

// Generates logical AND and puts the result (0 | 1) in the left register
void code_generator_and(int left_register, int right_register);

// Generates division operation between left and right registers. left_register = left_register / right_register,
void code_generator_divide(int left_register, int right_register);

// Generates modulu operation between left and right registers. left_register = left_register % right_register,
void code_generator_modulu(int left_register, int right_register);

// Generate logical NOT and puts the result (0 | 1) back in the register
void code_generator_not(int register_number);
