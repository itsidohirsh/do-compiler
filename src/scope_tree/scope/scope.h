#pragma once

#include "../symbol_table/symbol_table.h"


/* ---------- Structs ---------- */

// Struct of a single node in the tree, scope, in the tree of symbol tables scopes
typedef struct Scope
{
    // Current scope's symbol table
    Symbol_Table* symbol_table;
    // Array of scopes which are the children of the current scope.
    // Each child represent a sub scope of the current scope.
    struct Scope** children;
    int num_of_children;        // Length of the children array
    int current_child_index;    // Index of the current child in children array
    // Pointer to the current scope's parent in the symbol tables scopes tree
    struct Scope* parent;
} Scope;


/* ---------- Functions ---------- */

// Creates a scope with an empty symbol table and the given parent pointer
Scope* scope_init(Scope* parent);
