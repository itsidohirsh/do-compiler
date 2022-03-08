#pragma once

#include <stdbool.h>

#include "../../token/token.h"


// Number of character in the language - 128 ascii characters
#define NUM_OF_CHARACTERS 128
// Number of states in the lexer's Finite State Machine
#define LEXER_FSM_NUM_OF_STATES 64


/* ---------- Structs ---------- */

// Struct of a state in the lexer's FSM
typedef struct Lexer_State
{
    // Every state holds a token type, so if we stoped on that state we would know what token we have
    Token_Kind token_type;
} Lexer_State;

// Struct of a connection between states in the lexer's FSM
typedef struct Lexer_Edge
{
    // An edge is representing a transition between states in the FSM
    // Every edge is holding which state to go next
    char state_number;
} Lexer_Edge;

// Struct of the lexer's FSM
typedef struct Lexer_FSM
{
    // An array of starting state indices to help us know at which state to start when we start a new token
    int starting_state_indices[NUM_OF_CHARACTERS];
    // The array of states of the FSM
    Lexer_State states[LEXER_FSM_NUM_OF_STATES];
    // The matrix of edges of the FSM
    Lexer_Edge edges[LEXER_FSM_NUM_OF_STATES][NUM_OF_CHARACTERS];
} Lexer_FSM;


/* ---------- Functions ---------- */

// Creates a new lexer fsm on the heap and returns a pointer to it
Lexer_FSM* lexer_fsm_create();

// Frees everything we've allocated in the lexer_fsm_create() function
void lexer_fsm_destroy(Lexer_FSM* fsm);

// Initializes the FSM of the lexer
// Creates all the states and connections between them to make the FSM work properlly
void lexer_fsm_init(Lexer_FSM* fsm);

// Sets an edge between state and every alpha-numeric character in the adjacency matrix except for the except char
void lexer_fsm_set_alnum_identifier(Lexer_FSM* fsm, int state, char except);

// Returns the index of a certain character in the FSM adjacency matrix
int lexer_fsm_get_char_index(char value);

// Returns The index of the starting state of a token according to the current character
int lexer_fsm_get_starting_state_index(Lexer_FSM* fsm, char value);

// Adds a starting state index to the lexer's starting state indices array
void lexer_fsm_add_starting_state_index(Lexer_FSM* fsm, int char_index, int starting_state_index);

// Adds a state to the lexer's FSM
void lexer_fsm_add_state(Lexer_FSM* fsm, int state_number, Token_Kind token_type);

// Adds a connection to the lexer's FSM
void lexer_fsm_add_edge(Lexer_FSM* fsm, int state, int ch, char to_state);

// Prints the vertices & adjacency matrix of the graph that is representing the FSM
void lexer_fsm_print(Lexer_FSM* fsm);
