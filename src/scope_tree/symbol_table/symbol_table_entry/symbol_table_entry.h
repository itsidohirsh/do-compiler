#pragma once

#include "../symbol_table_base.h"
#include "../../../semantic/semantic.h"


/* ---------- Structs ---------- */

// Struct of an entry in the symbol table
typedef struct Symbol_Table_Entry
{
    Entry_Type entry_type;                  // The type of the entry - variable, function, ...
    char* identifier;                       // The identifier name which is the key for each entry
    Data_Type data_type;                    // The type of the identifier - int, char, ...
    struct Symbol_Table_Entry* next_entry;  // Pointer to the next entry in the linked list of entries in that particular index
} Symbol_Table_Entry;


/* ---------- Functions ---------- */

// Creates a new symbol table entry with the specified properties
Symbol_Table_Entry* symbol_table_entry_init(Entry_Type entry_type, char* identifier, Data_Type data_type);

// Frees memory allocated for an entry
void symbol_table_entry_destroy(Symbol_Table_Entry* entry);

// Prints an entry in a nice format
void symbol_table_entry_print(Symbol_Table_Entry* entry);
