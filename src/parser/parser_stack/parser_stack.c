#include <stdlib.h>
#include <stdio.h>

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

    // Update entry properties
    entry->tree_node = tree_node;
    entry->goto_state = goto_state;
    // entry->line = line;
    entry->next = NULL;

    return entry;
}

void parser_stack_destroy_entry(Parser_Stack_Entry* entry)
{
    // Free the stack entry's tree
    parse_tree_destroy_tree(entry->tree_node);
    // Free the stack entry
    free(entry);
}

void parser_stack_destroy_stack(Parser_Stack_Entry** head)
{
    // While the stack is not empty
    while (*head != NULL)
        // Pop and destroy the entry
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
    // Extract the top node
    Parser_Stack_Entry* poped_entry = *head;
    // Advance the head of the stack list
    *head = (*head)->next;
    // Disconect it from the stack list
    poped_entry->next = NULL;

    return poped_entry;
}

void parser_stack_print_stack(Parser_Stack_Entry* head)
{
    while (head != NULL)
    {
        printf("%d -> ", head->goto_state);
        head = head->next;
    }
    printf("\n");
}
