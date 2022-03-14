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
Scope_Tree* scope_tree_create();

// Destory the scope tree
void scope_tree_destroy(Scope_Tree* scope_tree);

// Frees all allocated memory in the tree of scopes
void scope_tree_destroy_tree(Scope* global_scope);
