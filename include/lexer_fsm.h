#pragma once

#include <stdbool.h>

#include "token.h"

// Number of states in the lexer's Finite State Machine
// All the charaters in the language are between the ascii values of '|' and ' '
#define LOWER_BOUND ' '
#define HIGHER_BOUND '~'
#define NUM_CHARACTERS (HIGHER_BOUND - LOWER_BOUND)
#define NUM_STATES 64

typedef enum State_Name
{
    IDENTIFIER = 1,
    NUMBER,
    P,
    PR,
    PRO,
    PROG,
    I,
    IF,
    E,
    EL,
    ELS,
    ELSE,
} State_Name;

/* ---------- Structs ---------- */

// Struct of a state in the lexer's FSM
typedef struct Lexer_State
{
    char value;
    Token_Type token_type;
    bool is_final_state;
} Lexer_State;

// Struct of a connection between states in the lexer's FSM
typedef struct Lexer_Edge
{
    char weight;
} Lexer_Edge;

// Struct of the lexer's FSM
typedef struct Lexer_FSM
{
    int num_states;
    Lexer_State states[NUM_STATES];
    Lexer_Edge edges[NUM_STATES][NUM_CHARACTERS];
} Lexer_FSM;

/* ---------- Functions ---------- */

// Initializes the FSM of the lexer
// Creates all the states and connections between them to make the FSM work properlly
void lexer_fsm_init(Lexer_FSM* fsm);

//
void lexer_fsm_alnum_to_identifier(Lexer_FSM* fsm, State_Name state, char except);

// Adds a state to the lexer's FSM
void lexer_fsm_add_state(Lexer_FSM* fsm, int index, char value, Token_Type token_type, bool is_final_state);

// Adds a connection to the lexer's FSM
void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char weight);

// Returns the index of a certain value in the vector of states of the lexer's FSM
int lexer_fsm_get_char_index(char value);

//
int lexer_fsm_get_state_index(char value);

// Prints the vertices & adjacency matrix of the graph that is representing the FSM
void lexer_fsm_print(Lexer_FSM* fsm);

// 
void lexer_fsm_print_dot(Lexer_FSM* fsm);
