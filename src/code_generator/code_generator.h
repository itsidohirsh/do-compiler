#pragma once

#include <stdbool.h>

#include "../parser/parse_tree/parse_tree.h"


// The maximum length of register's name
#define REGISTER_NAME_LENGTH 16
// The number of registers the code generator uses
#define NUM_OF_REGISTERS 8


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
} Code_Generator;


/* ---------- Functions ---------- */

// Create a new code generator and points the compiler's code generator to it
void code_generator_create();

// Frees the memory allocated for the code generator of the compiler
void code_generator_destroy();

// Initializes the registers array of the compilers code generator
void code_generator_init();

// Generates the assembly code for the given parse tree
void code_generator_generate(Parse_Tree_Node* parse_tree);

//
void code_generator_block(Parse_Tree_Node* block);

//
void code_generator_stmt(Parse_Tree_Node* stmt);

//
void code_generator_decl(Parse_Tree_Node* decl);

//
void code_generator_assign(Parse_Tree_Node* assign);

//
void code_generator_if_else(Parse_Tree_Node* if_else);

//
void code_generator_else(Parse_Tree_Node* _else);

//
void code_generator_while(Parse_Tree_Node* _while);

//
void code_generator_l_log_expr(Parse_Tree_Node* l_log_expr);

//
void code_generator_h_log_expr(Parse_Tree_Node* h_log_expr);

//
void code_generator_bool_expr(Parse_Tree_Node* bool_expr);

//
void code_generator_expr(Parse_Tree_Node* expr);

//
void code_generator_term(Parse_Tree_Node* term);

//
void code_generator_factor(Parse_Tree_Node* factor);
