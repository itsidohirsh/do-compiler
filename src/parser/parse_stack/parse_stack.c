#include <stdlib.h>

#include "parse_stack.h"
#include "../../error_handler/error_handler.h"


Parse_Stack_Entry* parse_stack_init_entry(Parse_Tree_Node* tree, int goto_state)
{
    // Create a new stack entry
    Parse_Stack_Entry* entry = (Parse_Stack_Entry*) calloc(1, sizeof(Parse_Stack_Entry));
    // Check for allocation error
    if (entry == NULL)
    {
        parse_tree_destroy(tree);
        error_handler_report_memory_error();
    }

    // Update entry properties
    entry->tree = tree;
    entry->goto_state = goto_state;
    entry->next_entry = NULL;

    return entry;
}

void parse_stack_destroy(Parse_Stack_Entry** stack)
{
    Parse_Stack_Entry* entry;

    // While the stack is not empty
    while (*stack != NULL)
    {
        // Pop and destroy the entry
        entry = parse_stack_pop(stack);

        // Free entry if not NULL
        if (entry != NULL)
        {
            parse_tree_destroy(entry->tree);
            free(entry);
        }
    }
}

void parse_stack_push(Parse_Stack_Entry** stack, Parse_Stack_Entry* entry)
{
    // Connect the entry as the stack of the stack
    entry->next_entry = *stack;
    // Make stack point to the new entry
    *stack = entry;
}

Parse_Stack_Entry* parse_stack_pop(Parse_Stack_Entry** stack)
{
    // Check for NULL pointer
    if (*stack == NULL)
        return NULL;

    // Extract the top node
    Parse_Stack_Entry* top_entry = *stack;
    // Advance the stack of the stack list
    *stack = (*stack)->next_entry;
    // Disconect it from the stack list
    top_entry->next_entry = NULL;

    return top_entry;
}
