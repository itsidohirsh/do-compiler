#pragma once

#include "scope/scope.h"
#include "symbol_table/symbol_table_entry/symbol_table_entry.h"


/* ---------- Structs ---------- */

// Struct of the symbol tables scope tree.
// This is the struct we will use as our symbol table.
// The tree represents the hierarchy of scopes in the source program.
typedef struct Scope_Tree
{
    Scope* global_scope;   // The root of the tree - the global scope
    Scope* current_scope;  // The current scope we are at
} Scope_Tree;


/* ---------- Functions ---------- */

// Create a new symbol tables scopes tree, initialized with an empty global scope
void scope_tree_create();

// Destory the scope tree
void scope_tree_destroy();

// Frees all allocated memory in the tree of scopes
void scope_tree_destroy_tree(Scope* global_scope);

// Creates a new scope child for the current scope in the scope tree.
void scope_tree_add_scope();

// Moves the current scope in the scope tree to point to its parent scope
void scope_tree_goto_parent();

// Moves the current scope in the scope tree to point to its next child scope.
// Also updates the current scope's current child to the next child.
void scope_tree_goto_child();

// Travers the tree from the current scope up to the global scope and tries to find the given
// identifier. If managed to find it, returns a pointer to it. Otherwise return NULL.
Symbol_Table_Entry* scope_tree_fetch(char* identifier);

// Resets the current_child_index field to be STARTING_CHILD_INDEX for every scope in the scope tree.
// Used before code generation so I could traverse the scope tree the same as in the parsing stage.
void scope_tree_reset_child_index(Scope* global_scope);

// Prints the scope tree in a nice format
void scope_tree_print();
