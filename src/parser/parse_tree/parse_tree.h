#pragma once

#include "../parser_base.h"
#include "../../token/token.h"
#include "../../semantic/semantic.h"


/* ---------- Structs ---------- */

// Struct of a node in the parse tree
typedef struct Parse_Tree_Node
{
    Symbol_Type symbol_type;            // Terminal / Non-Terminal
    int symbol;                         // The terminal or non-terminal kind of the current node
    Token* token;                       // If a node is a terminal then it will have a token, otherwise it will be NULL
    struct Parse_Tree_Node** children;  // Array of a node pointers which represents a node's children
    int num_of_children;                // The length of the childrens array
    // Attribute of a node in the tree.
    // If the node is part of expression it will have a data type.
    Data_Type data_type;
    // The register index in the register array of the code generator that holds this node's result.
    // If the node is part of expression it will have a register holding it's result.
    int register_number;
} Parse_Tree_Node;


/* ---------- Functions ---------- */

// Creates a new Parse_Tree_Node with the given attributes and returns a pointer to it
Parse_Tree_Node* parse_tree_init_node(Symbol_Type symbol_type, int symbol, Token* token, Parse_Tree_Node** children, int num_of_children);

// Recursive function to free a node and all of it's children trees
void parse_tree_destroy(Parse_Tree_Node* root);

// Prints the parse tree in a nice format
void parse_tree_print(Parse_Tree_Node* root);
