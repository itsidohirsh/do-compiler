#include <stdlib.h>
#include <stdio.h>

#include "symbol_table_entry.h"
#include "../../../error_handler/error_handler.h"


Symbol_Table_Entry* symbol_table_entry_init(Entry_Type entry_type, char* identifier, Data_Type data_type, void* value)
{
    // Create entry
    Symbol_Table_Entry* entry = (Symbol_Table_Entry*) calloc(1, sizeof(Symbol_Table_Entry));
    // Check for allocation error
    if (entry == NULL)
    {
        // If allocation error occured, free allocated memory and report allocation error
        free(identifier);
        free(value);

        error_handler_report_memory_error();
    }

    // Update entry's properties
    entry->entry_type = entry_type;
    entry->identifier = identifier;
    entry->data_type = data_type;
    entry->value = value;
    entry->next_entry = NULL;

    return entry;
}

void symbol_table_entry_destroy(Symbol_Table_Entry* entry)
{
    // check for NULL pointer
    if (entry != NULL)
    {
        free(entry->identifier);
        free(entry->value);
        free(entry);
    }
}

void symbol_table_entry_print(Symbol_Table_Entry* entry)
{
    // Check for NULL pointer
    if (entry == NULL)
        return;

    if (entry->data_type == Data_Type_Int)
    {
        printf("int %s = ", entry->identifier);
        printf("%d", *((int*) entry->value));
    }

    else
    {
        printf("char %s = ", entry->identifier);
        printf("'%c'", *((char*) entry->value));
    }
}
