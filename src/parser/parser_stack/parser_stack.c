#include <stdlib.h>

#include "parser_stack.h"
#include "../../error_handler/error_handler.h"

Parser_Stack_Entry* parser_stack_init_entry(Parse_Tree_Node* tree_node, int goto_state)
{
    // Create a new stack entry
    Parser_Stack_Entry* entry = (Parser_Stack_Entry*) calloc(1, sizeof(Parser_Stack_Entry));
    // Check for allocation error
    if (entry == NULL)
    {
        parse_tree_destroy_tree(tree_node);
        error_handler_report_alloc();
    }

    entry->tree_node = tree_node;
    entry->goto_state = goto_state;
    entry->next = NULL;

    return entry;
}

void parser_stack_destroy_entry(Parser_Stack_Entry* entry)
{
    parse_tree_destroy_tree(entry->tree_node);
    free(entry);
}

void parser_stack_destroy_stack(Parser_Stack_Entry** head)
{
    // While the stack is not empty
    while (*head != NULL)
        // Pop aand destroy the entry
        parser_stack_destroy_entry(parser_stack_pop(head));
}

void parser_stack_push(Parser_Stack_Entry** head, Parser_Stack_Entry* entry)
{
    // Connect the entry as the head of the stack
    entry->next = *head;
    // Make head point to the new entry
    *head = entry;
}

Parser_Stack_Entry* parser_stack_pop(Parser_Stack_Entry** head)
{
    // Extract the top node and disconect it from the stack list
    Parser_Stack_Entry* poped_entry = *head;
    poped_entry->next = NULL;

    // Advance the head of the stack list
    *head = (*head)->next;

    return poped_entry;
}
