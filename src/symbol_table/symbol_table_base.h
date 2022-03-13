#pragma once


// The initial capacity of a symbol table when creating it
#define SYMBOL_TABLE_INITIAL_CAPACITY 16


// Enum of all the possible types of entries in the symbol table.
// In my language this is really not that necessary because all the identifiers
// are variables. But I added it so I will have the option in the future to support
// other types of identifiers, functions for example.
typedef enum Entry_Type
{
    Entry_Type_Variable
} Entry_Type;

// Enum of all the possible types of an identifier in the language
typedef enum Data_Type
{
    Data_Type_Int, // int
    Data_Type_Char // char
} Data_Type;
