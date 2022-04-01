#include <stdlib.h>

#include "scope.h"
#include "../../general/general.h"


Scope* scope_init(Scope* parent)
{
    // Create a new scope
    Scope* scope = (Scope*) calloc(1, sizeof(Scope));
    if (scope == NULL) exit_memory_error(__FILE__, __LINE__);

    // Initialize without children
    scope->children = NULL;
    scope->num_of_children = 0;
    scope->current_child_index = STARTING_CHILD_INDEX;

    // Set initial value of available entries to 0
    scope->available_entries = 0;

    // Create empty symbol table
    scope->symbol_table = symbol_table_create();

    // Set parent
    scope->parent = parent;

    return scope;
}
