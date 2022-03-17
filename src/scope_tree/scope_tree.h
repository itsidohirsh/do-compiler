#pragma once

#include "scope/scope.h"


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

// Creates a new scope child for the given scope tree's current scope
// and advances the scope tree's current scope to that new scope.
void scope_tree_add_scope();

// Moves the current scope in the scope tree to point to its parent scope
void scope_tree_goto_parent();

// Moves the current scope in the scope tree to point to its next child scope.
// Also updates the current scope's next child to the next child.
void scope_tree_goto_child();
