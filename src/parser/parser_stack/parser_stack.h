#pragma once

#include "../../parse_tree/parse_tree.h"

/* ---------- Structs ---------- */

// The struct of a entry of the parser's stack.
// The stack is represents by a linear linked list, where the head of the list is the top of the stack.
typedef struct Parser_Stack_Entry
{
    Parse_Tree_Node* tree_node;         // Each entry of the stack holds a pointer to a tree node
    int goto_state;                     // The state to go to in the next iteration of the parser
    struct Parser_Stack_Entry* next;    // The next entry of the stack, the one on the bottom of the current one
} Parser_Stack_Entry;


/* ---------- Functions ---------- */

// Create a new stack entry, updates its properties, and returns a pointer to it
Parser_Stack_Entry* parser_stack_init_entry(Parse_Tree_Node* tree_node, int goto_state);

// Frees a single stack entry
void parser_stack_destroy_entry(Parser_Stack_Entry* entry);

// Frees the whole stack
void parser_stack_destroy_stack(Parser_Stack_Entry** head);

// Pushes a stack entry onto the stack
void parser_stack_push(Parser_Stack_Entry** head, Parser_Stack_Entry* entry);

// Pops the top entry from the stack and returns the pointer to it
Parser_Stack_Entry* parser_stack_pop(Parser_Stack_Entry** head);

// Prints the stack in a nice format
void parser_stack_print_stack(Parser_Stack_Entry* head);
