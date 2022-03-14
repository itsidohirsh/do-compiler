#pragma once

#include "parse_tree/parse_tree.h"
#include "parse_table/parse_table.h"
#include "parse_stack/parse_stack.h"
#include "../lexer/lexer.h"
#include "../symbol_table/scope_tree/scope_tree.h"


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
    // The parser's lexer, used to get the next token from the source code
    Lexer* lexer;
    // The parser's parsing table, made from action & goto tables
    // helps up to know which action to perform according to the next token
    Parse_Table* parse_table;
    // The parser's stack
    Parse_Stack_Entry* parse_stack;
    // Array of the production rules
    // This will be used when we reduce by a production rule in the parsing phase
    Production_Rule production_rules[NUM_OF_PRODUCTION_RULES];
} Parser;


/* ---------- Functions ---------- */

// Creates a new parser on the heap and returns a pointer to it
Parser* parser_create();

// Frees everything we've allocated in the parser_create() function
void parser_destroy(Parser* parser);

// Initializes the parser. Lexer, parse table, stack, and production rules
void parser_init(Parser* parser, char* src);

// Initializes the production rules array according to the grammar of the language
void parser_init_production_rules(Production_Rule* production_rules);

// Shifts the token and goto state onto the parse stack
void parser_shift(Parser* parser, Token* token, int goto_state);

// Reduce by the production rule
void parser_reduce(Parser* parser, int production_rule_num);

// Parses the source code and returns an Abstract Syntax Tree / Parse Tree that represents the source code.
// Also updates the symbol tables scopes tree that is given to it.
Parse_Tree_Node* parser_parse(Parser* parser, char* src, Scope_Tree* scope_tree);
