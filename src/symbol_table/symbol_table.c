#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"
#include "../error_handler/error_handler.h"


Symbol_Table* symbol_table_create()
{
    // Create a new symbol table
    Symbol_Table* symbol_table = (Symbol_Table*) calloc(1, sizeof(Symbol_Table));
    // Check for allocation error
    if (symbol_table == NULL) error_handler_report_memory_error();

    // Initial symbol table properties
    symbol_table->capacity = SYMBOL_TABLE_INITIAL_CAPACITY;
    symbol_table->num_of_entries = 0;
    symbol_table->num_or_indices_occupied = 0;

    // Create symbol table entries array
    symbol_table->entries = (Symbol_Table_Entry**) calloc(symbol_table->capacity, sizeof(Symbol_Table_Entry*));
    // Check for allocation error
    if (symbol_table->entries == NULL)
    {
        free(symbol_table);
        error_handler_report_memory_error();
    }

    return symbol_table;
}

void symbol_table_destroy(Symbol_Table* symbol_table)
{
    Symbol_Table_Entry* cur_entry;
    Symbol_Table_Entry* prev_entry;

    // Free symbol table entries
    for (int i = 0; i < symbol_table->num_of_entries; i++)
    {
        // For each index, free all the entries in it
        cur_entry = symbol_table->entries[i];
        while (cur_entry != NULL)
        {
            prev_entry = cur_entry;
            cur_entry = cur_entry->next_entry;
            symbol_table_entry_destroy(prev_entry);
        }
    }

    // Free entries array
    free(symbol_table->entries);

    // Free symbol table
    free(symbol_table);
}

uint64_t symbol_table_hash(char* identifier)
{
    uint64_t hash = FNV_OFFSET;
    while (*identifier != '\0')
    {
        hash ^= (uint64_t) (unsigned char) (*identifier);
        hash *= FNV_PRIME;
        identifier++;
    }

    return hash;
}

void symbol_table_insert(Symbol_Table* symbol_table, Symbol_Table_Entry* entry)
{
    // Get index of the entry in the table
    uint64_t hash = symbol_table_hash(entry->identifier);
    int index = (int) (hash % (uint64_t) (symbol_table->capacity - 1));

    // TODO: Check if need to expand table

    // Check if added to new index, if so adds 1 to the number of indices occupied in the table
    if (symbol_table->entries[index] == NULL)
        symbol_table->num_or_indices_occupied++;

    // Insert the entry into the table
    entry->next_entry = symbol_table->entries[index];
    symbol_table->entries[index] = entry;

    // Increment the number of entries in the table
    symbol_table->num_of_entries++;
}

Symbol_Table_Entry* symbol_table_fetch(Symbol_Table* symbol_table, char* identifier)
{
    // Get index of the entry in the table
    uint64_t hash = symbol_table_hash(identifier);
    int index = (int) (hash % (uint64_t) (symbol_table->capacity - 1));

    Symbol_Table_Entry* entry = symbol_table->entries[index];

    // Go over all the entries in that index and find the wanted entry
    while (entry != NULL)
    {
        // If found wated entry then return it
        if (strcmp(entry->identifier, identifier) == 0)
            return entry;
    }

    // If entry was not found, return NULL
    return NULL;
}

void symbol_table_print(Symbol_Table* symbol_table)
{
    printf("%d entries in total\n", symbol_table->num_of_entries);
    printf("%d / %d indices are occupied\n\n", symbol_table->num_or_indices_occupied, symbol_table->capacity);

    Symbol_Table_Entry* entry;
    for (int i = 0; i < symbol_table->capacity; i++)
    {
        entry = symbol_table->entries[i];

        if (entry == NULL)
            printf("----------");
        else
            while (entry != NULL)
            {
                printf("[");
                symbol_table_entry_print(entry);
                printf("] -> ");
                entry = entry->next_entry;
            }

        printf("\n");
    }
}
