#pragma once

#include "parsing_table_base.h"


/* ---------- Structs ---------- */

// Struct for each cell in the Action table
typedef struct Action_Table_Cell
{
    // Error, Shift, Reduce, Accept
    Action_Kind action_kind;
    // If the current cell is a Shift, then this int represents the state to go next
    // If the current cell is a Reduce, then this int represents the number of the production rule to reduce by
    int state_or_rule;
    // Pointer to a function to be called when we reach that cell. Will also be part of the semantic analysis
    void (*action_func) (); // TODO: Update function pointer for Action_Table_Cell struct
} Action_Table_Cell;

// Struct to hold the parsing table. Action & Goto tables all together
typedef struct Parsing_Table
{
    // Action table. each cell contains a Action_Table_Cell structure
    Action_Table_Cell action_table[PARSING_TABLE_NUM_OF_STATES][NUM_OF_TERMINALS];
    // Goto table. Each cell contains only an int that represent a state number
    int goto_table[PARSING_TABLE_NUM_OF_STATES][NUM_OF_NON_TERMINALS];
} Parsing_Table;


/* ---------- Functions ---------- */

// Creates a new Parsing_Table on the heap and returns a pointer to it
Parsing_Table* parsing_table_create();

// Frees everything we've allocated the parsing_table_create() function
void parsing_table_destroy(Parsing_Table* parsing_table);

// Initializes the parsing table according to the grammar rules
void parsing_table_init(Parsing_Table* parsing_table);

// Returns the index of a terminal in the action table
int parsing_table_get_terminal_index(Terminal_Kind terminal_kind);

// Inserts a new cell into the action table
void parsing_table_insert_action(Parsing_Table* parsing_table, int state, int terminal_index, Action_Table_Cell cell_contents);

// Inserts a new cell into the goto table
void parsing_table_insert_goto(Parsing_Table* parsing_table, int state, int non_terminal_index, int goto_state);

// Prints the parsing table in a nice format
void parsing_table_print(Parsing_Table* parsing_table);
