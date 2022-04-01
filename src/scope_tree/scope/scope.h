#pragma once

#include "../symbol_table/symbol_table.h"


// The starting value of the current_child_index field
#define STARTING_CHILD_INDEX -1

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
    // Number of entries seen up to this point.
    // In this scopeand in all the scope on the path up to the root scope.
    // Used in the code generation process in the address computation for variables.
    int available_entries;
} Scope;


/* ---------- Functions ---------- */

// Creates a scope with an empty symbol table and the given parent pointer
Scope* scope_init(Scope* parent);
