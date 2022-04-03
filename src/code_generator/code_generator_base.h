#pragma once

#include "../lexer/lexer.h"


/* ---------- Registers ---------- */

// The number of registers the code generator uses
#define NUM_OF_REGISTERS 7

// The maximum length of register's name
#define REGISTER_NAME_LENGTH 4

#define RBX "RBX"
#define R10 "R10"
#define R11 "R11"
#define R12 "R12"
#define R13 "R13"
#define R14 "R14"
#define R15 "R15"


/* ---------- Labels ---------- */

// The format of a lable.
// Expecting label number for the %d.
#define LABEL_FORMAT "$$L%d"

// The maximum length of lables's name
#define LABEL_NAME_LENGTH 8


/* ---------- Symbols Addresses ---------- */

// The format of an address in the data segment (global variables).
// Added $ at the start of the address to solve name collisions for global variables.
// Expecting the identifier name for the %s.
#define GLOBAL_ADDRESS_FORMAT "$%s"

// The format of an address on the stack (relative to rbp).
// Expecting the relative address off of rbp for the %d.
#define STACK_ADDRESS_FORMAT "[RBP - %d]"

// The maximum length of symbol address. It is equal to the maximum length of
// a token because the longest address that will be produced will be the length of
// a long identifier.
#define SYMBOL_ADDRESS_LENGTH LEXER_MAX_TOKEN_SIZE

// The size of an entry in the stack of the program.
// For x64 the size of a stack entry is 8 bytes
#define STACK_ENTRY_BYTES 8
