#pragma once

#include "parsing_table_base.h"


/* ---------- Structs ---------- */

typedef struct Parsing_Table_Cell
{
    // Error, Shift, Reduce, Accept
    Action_Type action_type;
    // If the current cell is a Shift, then this int represents the state to go next
    // If the current cell is a Reduce, then this int represents the number of the production rule to reduce by
    int state_or_rule;
    // Pointer to a function to be called when we reach that cell. Will also be part of the semantic analysis
    void (*action_func) (); // TODO: Update function pointer for Parsing_Table_Cell struct
} Parsing_Table_Cell;

typedef struct Parsing_Table
{
    Parsing_Table_Cell action_table[PARSING_TABLE_NUM_OF_STATES][NUM_OF_TERMINALS];
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
int terminal_index(Terminal_Type terminal_type);

// Inserts a new cell into the action table
void parsing_table_insert_action(Parsing_Table* parsing_table, int state, int terminal_index, Parsing_Table_Cell cell_contents);

// Inserts a new cell into the goto table
void parsing_table_insert_goto(Parsing_Table* parsing_table, int state, int non_terminal_index, int goto_state);

// Prints the parsing table in a nice format
void parsing_table_print(Parsing_Table* parsing_table);
