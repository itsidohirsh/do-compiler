#include "src/do/do.h"
#include "src/error_handler/error_handler.h"


int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf_red("Please specify source file.\n");
        return 1;
    }

    do_compile_file(argv[1]);

    return 0;
}


// // Symbol table check
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>

// #include "src/symbol_table/symbol_table.h"

// int main()
// {
//     Symbol_Table* symbol_table = symbol_table_create();

//     Symbol_Table_Entry* entry;
//     char* identifier;
//     void* value;

//     identifier = strdup("ido_hirsh_1");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 0;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("helloWorld");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 1;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("wow_wow_wow");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 2;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("if_done");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'a';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("lol");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'b';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("_L_");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'c';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("itayTuy");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'd';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("Tuti");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'e';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("ido_hirsh_2");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 0;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("helloWorld1");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 1;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("wow_wow_wo1w");
//     value = calloc(1, sizeof(int));
//     *((int*) value) = 2;
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Int, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("if_done1");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'a';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("lol1");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'b';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("_L_1");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'c';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     identifier = strdup("itayTuy1");
//     value = calloc(1, sizeof(char));
//     *((char*) value) = 'd';
//     entry = symbol_table_entry_init(Entry_Type_Variable, identifier, Data_Type_Char, value);
//     symbol_table_insert(symbol_table, entry);

//     symbol_table_print(symbol_table);
//     symbol_table_destroy(symbol_table);

//     return 0;
// }
