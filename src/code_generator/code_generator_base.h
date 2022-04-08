#pragma once

#include "../lexer/lexer.h"


/* ---------- Registers ---------- */

// The initial value of the register number for the different tree nodes
#define NO_REGISTER -1

// The number of registers the code generator uses
#define NUM_OF_REGISTERS 6

// The maximum length of register's name
#define REGISTER_NAME_LENGTH 8

// The registers used for code generation
#define R10 "r10"
#define R11 "r11"
#define R12 "r12"
#define R13 "r13"
#define R14 "r14"
#define R15 "r15"

// The registers used for some arithmetic operations (because they're the default result registers)
#define RAX "rax"
#define RDX "rdx"


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
#define STACK_ADDRESS_FORMAT "[rbp - %d]"

// The maximum length of symbol address. It is equal to the maximum length of
// a token because the longest address that will be produced will be the length of
// a long identifier.
#define SYMBOL_ADDRESS_LENGTH LEXER_MAX_TOKEN_SIZE

// The size of an entry in the stack of the program.
// For x64 the size of a stack entry is 8 bytes
#define STACK_ENTRY_BYTES 8


/* ---------- Instructions ---------- */

#define MOV     "\tmov  %s, %s\n"

// Control flow
#define CMP     "\tcmp  %s, %s\n"
#define LABEL   "%s:\n"
#define JMP     "\tjmp  %s\n"
#define JE      "\tje   %s\n"
#define JNE     "\tjne  %s\n"
#define JG      "\tjg   %s\n"
#define JGE     "\tjge  %s\n"
#define JL      "\tjl   %s\n"
#define JLE     "\tjle  %s\n"
#define JC      "\tjc   %s\n"
#define JNC     "\tjnc  %s\n"

// Arithmetic
#define ADD     "\tadd  %s, %s\n"
#define SUB     "\tsub  %s, %s\n"
#define SBB     "\tsbb  %s, %s\n"
#define IMUL    "\timul %s, %s\n"
#define IDIV    "\tidiv %s\n"
#define NEG     "\tneg  %s\n"


/* ---------- Data types ---------- */

#define DB " db ?\n"
#define DQ " dq ?\n"
