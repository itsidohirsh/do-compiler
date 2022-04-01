#pragma once

#include <stdbool.h>

#include "../symbol_table_base.h"
#include "../../../semantic/semantic.h"


/* ---------- Structs ---------- */

// Declare struct Scope (Defined in ../../scope/scope.h file)
struct Scope;

// Struct of an entry in the symbol table
typedef struct Symbol_Table_Entry
{
    Entry_Type entry_type;                  // The type of the entry - variable, function, ...
    char* identifier;                       // The identifier name which is the key for each entry
    Data_Type data_type;                    // The type of the identifier - int, char, ...
    // Pointer to the scope that entry is located at.
    // Used in the code generation process to go in O(1) to the scope of a given entry
    struct Scope* scope;
    // Whether that symbol is a global symbol or not.
    // Used in the code generation process to determine whether a variable
    // should be considred from the data segment of from the stack
    bool is_global;
    // The number of the entry in the current scope.
    // At which order it was entred. If it's the first, second, third, ...
    // Used in the code generation process to resolve variables addresses relative to bp
    int num_in_scope;
    struct Symbol_Table_Entry* next_entry;  // Pointer to the next entry in the linked list of entries in that particular index
} Symbol_Table_Entry;


/* ---------- Functions ---------- */

// Creates a new symbol table entry with the specified properties
Symbol_Table_Entry* symbol_table_entry_init(Entry_Type entry_type, char* identifier, Data_Type data_type);

// Frees memory allocated for an entry
void symbol_table_entry_destroy(Symbol_Table_Entry* entry);

// Prints an entry in a nice format
void symbol_table_entry_print(Symbol_Table_Entry* entry);
