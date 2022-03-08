#pragma once

#include <stdbool.h>

#include "../token/token.h"
#include "../parser/parsing_table/parsing_table_base.h"

/* ---------- Structs ---------- */

// Enum of the possible node types
typedef enum Parse_Tree_Node_Type
{
    Terminal,
    Non_Terminal,
} Parse_Tree_Node_Type;

// Struct of a node in the parse tree
typedef struct Parse_Tree_Node
{
    Parse_Tree_Node_Type node_type;     // Terminal / Non-Terminal / Epsilon
    int symbol;                         // The terminal or non-terminal kind of the current node
    Token* token;                       // If a node is a terminal then it will have a token, otherwise it will be NULL
    struct Parse_Tree_Node** children;  // Array of a node pointers which represents a node's children
    int num_of_children;                // The length of the childrens array
} Parse_Tree_Node;

/* ---------- Functions ---------- */

// Creates a new Parse_Tree_Node with the given attributes and returns a pointer to it
Parse_Tree_Node* parse_tree_init_node(Parse_Tree_Node_Type node_type, int symbol, Token* token, Parse_Tree_Node** children, int num_of_children);

// Recursive function to free a node and all of it's children trees
void parse_tree_destroy_tree(Parse_Tree_Node* root);

// Prints the parse tree in a nice format
void parse_tree_print_tree(Parse_Tree_Node* root);
// Converts a non-terminal kind to a string representation
const char* parser_tree_non_terminal_to_str(Non_Terminal_Kind terminal_kind);
// Prints a single node in the parse tree
void parse_tree_print_node(Parse_Tree_Node* node);
// The recursive function that actually prints the parse tree
void parse_tree_print_tree_rec(Parse_Tree_Node* root, char* indent, bool is_last);
