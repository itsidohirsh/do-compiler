#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../global.h"

#include "scope_tree.h"
#include "../general/general.h"


void scope_tree_create()
{
    // Create a new scope tree
    compiler.scope_tree = (Scope_Tree*) calloc(1, sizeof(Scope_Tree));
    if (compiler.scope_tree == NULL) exit_memory_error(__FILE__, __LINE__);

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
    if (compiler.scope_tree->current_scope->children == NULL) exit_memory_error(__FILE__, __LINE__);

    // Add the new scope to the last index of the children array of the current scope in the scope tree
    compiler.scope_tree->current_scope->children[compiler.scope_tree->current_scope->num_of_children - 1] = new_scope;
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
    // If there is not, the current scope remains as it is.
    if (compiler.scope_tree->current_scope->current_child_index + 1 <= compiler.scope_tree->current_scope->num_of_children - 1)
    {
        // Advance the current child to the next child
        compiler.scope_tree->current_scope->current_child_index++;

        // Advance the current scope to the next child scope
        compiler.scope_tree->current_scope = compiler.scope_tree->current_scope->children[compiler.scope_tree->current_scope->current_child_index];
    }
}

Symbol_Table_Entry* scope_tree_fetch(char* identifier)
{
    Scope* current_scope = compiler.scope_tree->current_scope;
    Symbol_Table_Entry* entry = NULL;

    // Loop up to the global scope, or until identifier found
    while (current_scope != NULL && entry == NULL)
    {
        entry = symbol_table_fetch(current_scope->symbol_table, identifier);
        current_scope = current_scope->parent;
    }

    return entry;
}

void scope_tree_print_tree(Scope* global_scope, char* indent, bool is_last);

void scope_tree_print()
{
    char indent[256] = { 0 };
    scope_tree_print_tree(compiler.scope_tree->global_scope, indent, true);
}

void scope_tree_print_tree(Scope* global_scope, char* indent, bool is_last)
{
    if (global_scope == NULL)
        return;

    char marker[256] = " |---";
    char cur_indent[256] = " |   ";

    if (is_last)
    {
        sprintf(marker, " `---");
        sprintf(cur_indent, "     ");
    }

    printf("%s", indent);
    printf("%s", marker);

    printf(" ");
    printf("Entries: %d", global_scope->symbol_table->num_of_entries);
    printf("\n");

    strcat(indent, cur_indent);

    // Save current indentation before recursing
    char tmp[256];
    strcpy(tmp, indent);

    for (int i = 0; i < global_scope->num_of_children; i++)
    {
        scope_tree_print_tree(global_scope->children[i], indent, i == global_scope->num_of_children - 1);
        strcpy(indent, tmp);
    }
}
