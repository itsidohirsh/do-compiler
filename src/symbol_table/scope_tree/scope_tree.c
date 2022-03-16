#include <stdlib.h>

#include "scope_tree.h"
#include "../../error_handler/error_handler.h"

Scope_Tree* scope_tree_create()
{
    // Create a new scope tree
    Scope_Tree* scope_tree = (Scope_Tree*) calloc(1, sizeof(Scope_Tree));
    // Check for allocation error
    if (scope_tree == NULL) error_handler_report_memory_error();

    // Create global scope with it's parent set to NULL (The only scope with NULL father)
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
    free(global_scope);
}

void scope_tree_add_scope(Scope_Tree* scope_tree)
{
    // Create a new scope and set its parent to the current scope in the scope tree
    Scope* new_scope = scope_init(scope_tree->current_scope);

    // Resizes the children array of the current scope in the scope tree, and add 1 to the number of children
    scope_tree->current_scope->children = (Scope**) realloc(scope_tree->current_scope->children, ++scope_tree->current_scope->num_of_children * sizeof(Scope*));
    // Check for allocation error
    if (scope_tree->current_scope->children == NULL)
    {
        scope_tree_destroy(scope_tree);
        error_handler_report_memory_error();
    }

    // Advance the current child to the next child
    scope_tree->current_scope->current_child_index++;

    // Add the new scope to the last index of the children array of the current scope in the scope tree
    scope_tree->current_scope->children[scope_tree->current_scope->current_child_index] = new_scope;

    // Advance the scope tree's current scope to the newly added scope in the children array
    scope_tree->current_scope = scope_tree->current_scope->children[scope_tree->current_scope->current_child_index];
}

void scope_tree_goto_parent(Scope_Tree* scope_tree)
{
    // Check if the current scope is the global scope.
    // If not, advance the current scope to the parent scope of the current scope.
    // If yes, remain at the global scope.
    if (scope_tree->current_scope->parent != NULL)
    {
        scope_tree->current_scope = scope_tree->current_scope->parent;
    }
}

void scope_tree_goto_child(Scope_Tree* scope_tree)
{
    // Check if there is a child to go to next.
    // If there is, advances the current scope's current_child_index and advances the current scope to the next child scope.
    // If there is not, remains in the current scope.
    if (scope_tree->current_scope->current_child_index + 1 <= scope_tree->current_scope->num_of_children - 1)
    {
        // Advance the current child to the next child
        scope_tree->current_scope->current_child_index++;

        // Advance the current scope to the next child scope
        scope_tree->current_scope = scope_tree->current_scope->children[scope_tree->current_scope->current_child_index];
    }
}
