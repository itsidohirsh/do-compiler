#pragma once

#include "parse_tree/parse_tree.h"
#include "parse_table/parse_table.h"
#include "parse_stack/parse_stack.h"
#include "../token/token.h"


// Number of production rules in the grammar of the language
#define NUM_OF_PRODUCTION_RULES 28


/* ---------- Structs ---------- */

// A struct to hold each production rule's length and non-terminal on its LHS
typedef struct Production_Rule
{
    // The non-terminal that is on the LHS of a production rule
    Non_Terminal_Type non_terminal_type;
    // The number of Terminals and Non-Terminals that are on the RHS of a production rule
    int rule_length;
} Production_Rule;

// Struct of the parser
typedef struct Parser
{
    // The current token from the source code produced by the lexer
    Token* token;
    // The parser's parsing table, made from action & goto tables
    // helps up to know which action to perform according to the next token
    Parse_Table* parse_table;
    // The parser's stack
    Parse_Stack_Entry* parse_stack;
    // Array of the production rules
    // This will be used when we reduce by a production rule in the parsing phase
    Production_Rule production_rules[NUM_OF_PRODUCTION_RULES];
    // Array to store the starting state of the tokens at the start of a statment production rule in the language.
    // This array is mainly used for better error reporting and error recovery.
    // Every cell is filled with 0s, except for the tokens that are at the start of a statment production rule.
    // Those token's cells are filled with the number of their produciton rule's starting state in the pushdown automaton
    // according to the parsing table.
    int tokens_starting_state[NUM_OF_TERMINALS];
} Parser;


/* ---------- Functions ---------- */

// Creates a new parser on the heap and returns a pointer to it
void parser_create();

// Frees everything we've allocated in the parser_create() function
void parser_destroy();

// Initializes the parser. Lexer, parse table, stack, and production rules
void parser_init();

// Initializes the production rules array according to the grammar of the language
void parser_init_production_rules();

// Initializes the array of statement production rule's starting states according to the parsing table of the language
void parser_init_tokens_starting_state();

// Get the starting state of a terminal
int parser_get_token_starting_state(Token_Type token_type);

// Shifts the current parser's token and goto state onto the parse stack
void parser_shift(int goto_state);

// Reduce by the production rule
void parser_reduce(int production_rule_num);

// Parses the source code and returns an Abstract Syntax Tree / Parse Tree that represents the source code.
// Also updates the symbol tables scopes tree that is given to it.
Parse_Tree_Node* parser_parse();
