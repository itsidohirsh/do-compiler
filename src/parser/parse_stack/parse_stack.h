#pragma once

#include "../parse_tree/parse_tree.h"


/* ---------- Structs ---------- */

// The struct of a entry of the parser's stack.
// The stack is represents by a linear linked list, where the stack of the list is the top of the stack.
typedef struct Parse_Stack_Entry
{
    Parse_Tree_Node* tree;          // Each entry of the stack holds a tree
    int goto_state;                 // The state to go to in the next iteration of the parser
    struct Parse_Stack_Entry* next_entry; // The next entry of the stack, the one on the bottom of the current one
} Parse_Stack_Entry;


/* ---------- Functions ---------- */

// Create a new stack entry, updates its properties, and returns a pointer to it
Parse_Stack_Entry* parse_stack_init_entry(Parse_Tree_Node* tree, int goto_state);

// Frees the whole stack
void parse_stack_destroy(Parse_Stack_Entry** stack);

// Pushes a stack entry onto the stack
void parse_stack_push(Parse_Stack_Entry** stack, Parse_Stack_Entry* entry);

// Pops the top entry from the stack and returns the pointer to it
Parse_Stack_Entry* parse_stack_pop(Parse_Stack_Entry** stack);
