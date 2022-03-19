#include <stdlib.h>
#include <stdio.h>

#include "symbol_table_entry.h"
#include "../../../general/general.h"
#include "../../../semantic/semantic.h"


Symbol_Table_Entry* symbol_table_entry_init(Entry_Type entry_type, char* identifier, Data_Type data_type)
{
    // Create entry
    Symbol_Table_Entry* entry = (Symbol_Table_Entry*) calloc(1, sizeof(Symbol_Table_Entry));
    if (entry == NULL)
    {
        free(identifier);
        exit_memory_error(__FILE__, __LINE__);
    }

    // Update entry's properties
    entry->entry_type = entry_type;
    entry->identifier = identifier;
    entry->data_type = data_type;
    entry->next_entry = NULL;

    return entry;
}

void symbol_table_entry_destroy(Symbol_Table_Entry* entry)
{
    // check for NULL pointer
    if (entry != NULL)
    {
        free(entry->identifier);
        free(entry);
    }
}

void symbol_table_entry_print(Symbol_Table_Entry* entry)
{
    // Check for NULL pointer
    if (entry == NULL)
        return;

    if (entry->data_type == Data_Type_Int)
        printf("int %s", entry->identifier);

    else if (entry->data_type == Data_Type_Char)
        printf("char %s", entry->identifier);

    else
        printf("Don't know that data type... ;|");
}
