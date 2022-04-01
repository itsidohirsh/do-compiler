#pragma once

#include <stdbool.h>

#include "../parser/parse_tree/parse_tree.h"


// The maximum length of register's name
#define REGISTER_NAME_LENGTH 16
// The maximum length of lables's name
#define LABEL_NAME_LENGTH 16
// The maximum length of symbol address
#define SYMBOL_ADDRESS_LENGTH 16
// The number of registers the code generator uses
#define NUM_OF_REGISTERS 7
// The size of an entry in the stack of the program.
// For x64 the size of a stack entry is 8 bytes
#define STACK_ENTRY_BYTES 8


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

// Searches for a free register in the code generator registers array.
// If found, marks it as inuse and returns the index of that register.
// If not found, terminates the compiler.
// TODO: Find better solution for not finding a free register
int code_generator_register_alloc();

// Marks the register in index r in the code generator registers array as unused
void code_generator_register_free(int r);

// Returns the register name of register r in the code generator registers array
char* code_generator_register_name(int r);

// Allocates a new lable name and returns a pointer to it
char* code_generator_label_create();

// Performs the right address computation for the given symbol according to its
// place in the program (globla / local),
// and returns an allocated string which represents that address.
// If the entry is NULL, returns NULL.
char* code_generator_symbol_address(Symbol_Table_Entry* entry);

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
