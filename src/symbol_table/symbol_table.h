#pragma once

#include <stdint.h>

#include "symbol_table_base.h"
#include "symbol_table_entry/symbol_table_entry.h"


/* ---------- Structs ---------- */

// Struct of a symbol table using chaining
typedef struct Symbol_Table
{
    Symbol_Table_Entry** entries;   // Array of the entry pointers of the symbol table
    int capacity;                   // Max capacity of symbol table
    int num_of_entries;             // Current number of entries in the symbol table
    int num_or_indices_occupied;    // Number of indices that have entries in them
} Symbol_Table;


/* ---------- Functions ---------- */

// Creates a new symbol table with initial capacity and return a pointer to it
Symbol_Table* symbol_table_create();

// Frees all memory allocated for a symbol table
void symbol_table_destroy(Symbol_Table* symbol_table);

// Constants used by the hash function
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Calculates and returns a 64-bit FNV-1a hash for the given identifier.
// See description: https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
uint64_t symbol_table_hash(char* identifier);

// Inserts an entry into the symbol table
void symbol_table_insert(Symbol_Table* symbol_table, Symbol_Table_Entry* entry);

// Returns a pointer to the entry in the symbol table that corresponds to the given identifier.
// If the identifier was not found, returns NULL
Symbol_Table_Entry* symbol_table_fetch(Symbol_Table* symbol_table, char* identifier);

// Prints the symbol table in a nice format
void symbol_table_print(Symbol_Table* symbol_table);
