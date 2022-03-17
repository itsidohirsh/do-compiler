#include <stdlib.h>

#include "../global.h"

#include "scope_tree.h"
#include "../error_handler/error_handler.h"

void scope_tree_create()
{
    // Create a new scope tree
    compiler.scope_tree = (Scope_Tree*) calloc(1, sizeof(Scope_Tree));
    // Check for allocation error
    if (compiler.scope_tree == NULL) error_handler_report_memory_error();

    // Create global scope with it's parent set to NULL (The only scope with NULL father)
    compiler.scope_tree->global_scope = scope_init(NULL);
    compiler.scope_tree->current_scope = compiler.scope_tree->global_scope;
}

void scope_tree_destroy()
{
    // Check for NULL pointer
    if (compiler.scope_tree != NULL)
    {
        scope_tree_destroy_tree(compiler.scope_tree->global_scope);
        free(compiler.scope_tree);
        compiler.scope_tree = NULL;
    }
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

void scope_tree_add_scope()
{
    // Create a new scope and set its parent to the current scope in the scope tree
    Scope* new_scope = scope_init(compiler.scope_tree->current_scope);

    // Resizes the children array of the current scope in the scope tree, and add 1 to the number of children
    compiler.scope_tree->current_scope->children = (Scope**) realloc(compiler.scope_tree->current_scope->children, ++compiler.scope_tree->current_scope->num_of_children * sizeof(Scope*));
    // Check for allocation error
    if (compiler.scope_tree->current_scope->children == NULL) error_handler_report_memory_error();

    // Advance the current child to the next child
    compiler.scope_tree->current_scope->current_child_index++;

    // Add the new scope to the last index of the children array of the current scope in the scope tree
    compiler.scope_tree->current_scope->children[compiler.scope_tree->current_scope->current_child_index] = new_scope;

    // Advance the scope tree's current scope to the newly added scope in the children array
    compiler.scope_tree->current_scope = compiler.scope_tree->current_scope->children[compiler.scope_tree->current_scope->current_child_index];
}

void scope_tree_goto_parent()
{
    // Check if the current scope is the global scope.
    // If not, advance the current scope to the parent scope of the current scope.
    // If yes, remain at the global scope.
    if (compiler.scope_tree->current_scope->parent != NULL)
    {
        compiler.scope_tree->current_scope = compiler.scope_tree->current_scope->parent;
    }
}

void scope_tree_goto_child()
{
    // Check if there is a child to go to next.
    // If there is, advances the current scope's current_child_index and advances the current scope to the next child scope.
    // If there is not, remains in the current scope.
    if (compiler.scope_tree->current_scope->current_child_index + 1 <= compiler.scope_tree->current_scope->num_of_children - 1)
    {
        // Advance the current child to the next child
        compiler.scope_tree->current_scope->current_child_index++;

        // Advance the current scope to the next child scope
        compiler.scope_tree->current_scope = compiler.scope_tree->current_scope->children[compiler.scope_tree->current_scope->current_child_index];
    }
}
