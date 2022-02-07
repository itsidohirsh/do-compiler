#pragma once

#include <stdbool.h>

#include "token.h"

// Number of character in the language
// The bounds are according to the smallest and largest character ascii value in the language
#define LOWER_BOUND 0
#define HIGHER_BOUND 128
#define NUM_OF_CHARACTERS (HIGHER_BOUND - LOWER_BOUND)

// Number of states in the lexer's Finite State Machine
#define NUM_OF_STATES 64


/* ---------- Structs ---------- */

// Struct of a state in the lexer's FSM
typedef struct Lexer_State
{
    Token_Type token_type;
} Lexer_State;

// Struct of a connection between states in the lexer's FSM
typedef struct Lexer_Edge
{
    char weight;
} Lexer_Edge;

// Struct of the lexer's FSM
typedef struct Lexer_FSM
{
    int starting_state_indices[NUM_OF_CHARACTERS];
    Lexer_State states[NUM_OF_STATES];
    Lexer_Edge edges[NUM_OF_STATES][NUM_OF_CHARACTERS];
} Lexer_FSM;


/* ---------- Functions ---------- */

// Initializes the FSM of the lexer
// Creates all the states and connections between them to make the FSM work properlly
void lexer_fsm_init(Lexer_FSM* fsm);

// 
void lexer_fsm_alnum_to_identifier(Lexer_FSM* fsm, int state, char except);

// Returns the index of a certain value in the vector of states of the lexer's FSM
int lexer_fsm_get_char_index(char value);

// Returns The index of the starting state of the current token
int lexer_fsm_get_state_index(Lexer_FSM* fsm, char value);

// Adds a starting state index to the lexer's state indices array
void lexer_fsm_add_state_index(Lexer_FSM* fsm, int char_index, int starting_state_index);

// Adds a state to the lexer's FSM
void lexer_fsm_add_state(Lexer_FSM* fsm, int state_number, Token_Type token_type);

// Adds a connection to the lexer's FSM
void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char weight);

// Returns true if from is adjacent to to, else returns false
bool lexer_fsm_is_adjacent(Lexer_FSM* fsm, int from, int to);

// Prints the vertices & adjacency matrix of the graph that is representing the FSM
void lexer_fsm_print(Lexer_FSM* fsm);
