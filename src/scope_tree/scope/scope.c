#include <stdlib.h>

#include "scope.h"
#include "../../error_handler/error_handler.h"


Scope* scope_init(Scope* parent)
{
    // Create a new scope
    Scope* scope = (Scope*) calloc(1, sizeof(Scope));
    // Check for allocation error
    if (scope == NULL) error_handler_report_memory_error();

    // Initialize without children
    scope->children = NULL;
    scope->num_of_children = 0;
    scope->current_child_index = -1;

    // Create empty symbol table
    scope->symbol_table = symbol_table_create();

    // Set parent
    scope->parent = parent;

    return scope;
}
