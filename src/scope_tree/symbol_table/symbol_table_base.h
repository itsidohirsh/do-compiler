#pragma once

#include "../../token/token.h"


// The initial capacity of a symbol table when creating it.
// Number of indices in the array.
#define SYMBOL_TABLE_INITIAL_CAPACITY 32

// TODO: Set λ limit value to the best value possible
// The maximum value the load factor of the hash table can have before expanding the table
#define SYMBOL_TABLE_MAX_LAMBDA 2.5


// Enum of all the possible types of entries in the symbol table.
// In my language this is really not that necessary because all the identifiers
// are variables. But I added it so I will have the option in the future to support
// other types of identifiers, functions for example.
typedef enum Entry_Type
{
    Entry_Type_Variable
} Entry_Type;

// Enum of all the possible types of an identifier in the language.
// This enum is only for readability and ease of use.
typedef enum Data_Type
{
    Data_Type_Int = Token_Int,      // int
    Data_Type_Char = Token_Char,    // char
} Data_Type;
