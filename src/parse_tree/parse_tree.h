#pragma once

#include "../token/token.h"

/* ---------- Structs ---------- */

// Enum of the possible node types
typedef enum Parse_Tree_Node_Type
{
    Terminal,
    Non_Terminal,
    Epsilon
} Parse_Tree_Node_Type;

// Struct of a node in the parse tree
typedef struct Parse_Tree_Node
{
    Parse_Tree_Node_Type node_type; // Terminal / Non-Terminal / Epsilon
    int symbol; // The terminal or non-terminal of the current node
    Token* token; // If a node is a terminal then it will have a token, otherwise it will be NULL
    struct Parse_Tree_Node** children; // Array of a node pointers which represents a node's children
    int num_of_children; // The length of the childrens array
} Parse_Tree_Node;

/* ---------- Functions ---------- */

// Creates a new Parse_Tree_Node with the given attributes and returns a pointer to it
Parse_Tree_Node* parse_tree_init_node(Parse_Tree_Node_Type node_type, int symbol, Token* token, Parse_Tree_Node** children, int num_of_children);

// Recursive function to free a node and all of it's children trees
void parse_tree_destroy_tree(Parse_Tree_Node* root);

// Prints the parse tree in a nice format
void parse_tree_print_tree(Parse_Tree_Node* root);
