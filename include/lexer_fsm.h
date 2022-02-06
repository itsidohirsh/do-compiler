#pragma once

#include <stdbool.h>

#include "token.h"

// Number of states in the lexer's Finite State Machine
// The bounds are according to the smallest and largest character ascii value in the language
// #define LOWER_BOUND 0
// #define HIGHER_BOUND 128
#define LOWER_BOUND ' '
#define HIGHER_BOUND '~'
#define NUM_CHARACTERS (HIGHER_BOUND - LOWER_BOUND)

// There are 64 states in the FSM of the lexer
#define NUM_STATES 64

// Final State Name
typedef enum FS_Name
{
    // End of token
    FS_Eot,
    // Whitespace
    FS_Whitespace,
    // Identifier
    FS_Identifier,
    // Number / Integer Literal
    FS_Number,
    // Character literal - the _ stands for '
    FS__,
    FS__C,
    FS__C_,
    // "int" & "if"
    FS_I,
    FS_iN,
    FS_inT,
    FS_iF,
    // "char"
    FS_C,
    FS_cH,
    FS_chA,
    FS_chaR,
    // "prog"
    FS_P,
    FS_pR,
    FS_prO,
    FS_proG,
    // "else"
    FS_E,
    FS_eL,
    FS_elS,
    FS_elsE,
    // "while"
    FS_W,
    FS_wH,
    FS_whI,
    FS_whiL,
    FS_whilE,
    // "set"
    FS_S,
    FS_sE,
    FS_seT,
    // "done"
    FS_D,
    FS_dO,
    FS_doN,
    FS_donE,
    // = & ==
    FS_Assignment,
    FS_Equal,
    //_ ! & !=
    FS_Not,
    FS_Not_Equal,
    // > & >=
    FS_Bigger,
    FS_Bigger_Equal,
    // < & <=
    FS_Smaller,
    FS_Smaller_Equal,
    // ||
    FS_or,
    FS_OR,
    // &&
    FS_and,
    FS_AND,
    // +
    FS_Plus,
    // -
    FS_Minus,
    // *
    FS_Multiply,
    // /
    FS_Divide,
    // %
    FS_Modulu,
    // (
    FS_Open_Paren,
    // )
    FS_Close_Paren,
    // :
    FS_Colon,
    // ;
    FS_Semi_Colon,
} FS_Name;


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

// Fills all edges of state that are alpha-numeric with the FS_Identifier value as weight, except the except char
void lexer_fsm_alnum_to_identifier(Lexer_FSM* fsm, FS_Name state, char except);

// Adds a state to the lexer's FSM
void lexer_fsm_add_state(Lexer_FSM* fsm, int index, char value, Token_Type token_type, bool is_final_state);

// Adds a connection to the lexer's FSM
void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char weight);

// Returns the index of a certain value in the vector of states of the lexer's FSM
int lexer_fsm_get_char_index(char value);

// Returns The index of the starting state of the current token
int lexer_fsm_get_state_index(char value);

// Prints the vertices & adjacency matrix of the graph that is representing the FSM
void lexer_fsm_print(Lexer_FSM* fsm);
