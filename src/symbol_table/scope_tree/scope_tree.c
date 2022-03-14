#include <stdlib.h>

#include "scope_tree.h"
#include "../../error_handler/error_handler.h"

Scope_Tree* scope_tree_create()
{
    // Create a new scope tree
    Scope_Tree* scope_tree = (Scope_Tree*) calloc(1, sizeof(Scope_Tree));
    // Check for allocation error
    if (scope_tree == NULL) error_handler_report_memory_error();

    // Create global scope with it's father set to NULL (The only scope with NULL father)
    scope_tree->global_scope = scope_init(NULL);
    scope_tree->current_scope = scope_tree->global_scope;

    return scope_tree;
}

void scope_tree_destroy(Scope_Tree* scope_tree)
{
    scope_tree_destroy_tree(scope_tree->global_scope);
    free(scope_tree);
}

void scope_tree_destroy_tree(Scope* global_scope)
{
    if (global_scope == NULL)
        return;

    // Destroy all of the current node's children
    for (int i = 0; i < global_scope->num_of_children; i++)
        scope_tree_destroy_tree(global_scope->children[i]);

    // Free current scope
    free(global_scope->children);
    symbol_table_destroy(global_scope->symbol_table);
}
