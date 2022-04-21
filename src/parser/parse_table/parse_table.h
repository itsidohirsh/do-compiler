#pragma once

#include "../parser_base.h"


/* ---------- Structs ---------- */

// Struct of an entry in the Action table
typedef struct Action
{
    // Error, Shift, Reduce, Accept
    Action_Type action_type;
    // If the current cell is a Shift, then this int represents the state to go next
    // If the current cell is a Reduce, then this int represents the number of the production rule to reduce by
    int state_or_rule;
    // Pointer to a function to be called when we reach that cell. Will also be part of the semantic analysis
    void (*semantic_func) ();
    // Pointer to a function to be called when we reach that cell. Takes part in the error reporting and recovery.
    void (*error_func) ();
} Action;

// Struct to hold the parsing table. Action & Goto tables all together
typedef struct Parse_Table
{
    // Action table. each cell contains a Action structure
    Action action_table[PARSING_TABLE_NUM_OF_STATES][NUM_OF_TERMINALS];
    // Goto table. Each cell contains only an int that represent a state number
    int goto_table[PARSING_TABLE_NUM_OF_STATES][NUM_OF_NON_TERMINALS];
} Parse_Table;


/* ---------- Functions ---------- */

// Creates a new Parse_Table on the heap and sets the Parser's parse table to point to it
void parse_table_create();

// Frees everything we've allocated the parse_table_create() function
void parse_table_destroy();

// Initializes the parsing table according to the grammar rules
void parse_table_init();

// Returns the index of a terminal in the action table
int parse_table_get_terminal_index(Terminal_Type terminal_type);

// Inserts a new cell into the action table
void parse_table_insert_action(int state, int terminal_index, Action action);

// Inserts a new cell into the goto table
void parse_table_insert_goto(int state, int non_terminal_index, int goto_state);

// Prints the parsing table in a nice format
void parse_table_print();
