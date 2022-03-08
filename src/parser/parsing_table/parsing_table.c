#include <stdlib.h>
#include <stdio.h>

#include "parsing_table.h"
#include "../../token/token.h"
#include "../../error_handler/error_handler.h"

Parsing_Table* parsing_table_create()
{
    // Create a new parsing table
    Parsing_Table* parsing_table = (Parsing_Table*) calloc(1, sizeof(Parsing_Table));
    // Check for allocation error
    if (parsing_table == NULL) error_handler_report_alloc();

    return parsing_table;
}

void parsing_table_destroy(Parsing_Table* parsing_table)
{
    free(parsing_table);
}

void parsing_table_init(Parsing_Table* parsing_table)
{
    // This function is dependent on the parsing table, Action & Goto, that I created before hand 
    // according to the grammar of the language.
    // I explanined how I created it very thoroughly in the strategy chapter of my project book.

    // The current state in the parsing table
    int s;
    // The current parsing table cell contents
    Parsing_Table_Cell cell_contents;

    // State 0
    s = 0;
    // - Action
    // -- `prog` -> S2
    cell_contents = (Parsing_Table_Cell) { Action_Shift, 2, NULL };
    parsing_table_insert_action(parsing_table, s, terminal_index(Token_Prog), cell_contents);
    // - Goto
    // -- PROG -> 1
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_PROG, 1);

    // State 1
    s = 1;
    // - Action
    // -- EOF -> Accept
    cell_contents = (Parsing_Table_Cell) { Action_Accept, 0, NULL };
    parsing_table_insert_action(parsing_table, s, terminal_index(Token_Eof), cell_contents);

    // State 2
    s = 2;
    // - Action
    // -- id -> S3
    cell_contents = (Parsing_Table_Cell) { Action_Shift, 3, NULL };
    parsing_table_insert_action(parsing_table, s, terminal_index(Token_Identifier), cell_contents);

    // State 3
    s = 3;
    // - Action
    // -- `:` -> S4
    cell_contents = (Parsing_Table_Cell) { Action_Shift, 4, NULL };
    parsing_table_insert_action(parsing_table, s, terminal_index(Token_Colon), cell_contents);

    // State 4
    s = 4;
    // - Action
    // -- ...

    // - Goto
    // -- ...

    // TODO: Complete parsing_table_init() function
}

int terminal_index(Terminal_Type terminal_type)
{
    // Returning the terminal_type - Token_Identifier because the tokens enum has 2 extra tokens at the start of it,
    // Token_Error & Token_Whitespace, which we don't care about here. We want to start the indexing from 0 and not from 2.
    return terminal_type - Token_Identifier;
}

void parsing_table_insert_action(Parsing_Table* parsing_table, int state, int terminal_index, Parsing_Table_Cell cell_contents)
{
    parsing_table->action_table[state][terminal_index] = cell_contents;
}

void parsing_table_insert_goto(Parsing_Table* parsing_table, int state, int non_terminal_index, int goto_state)
{
    parsing_table->goto_table[state][non_terminal_index] = goto_state;
}

void parsing_table_print(Parsing_Table* parsing_table)
{
    printf("\n");

    int state, symbol;

    // Print top row
    printf("     ");
    // Terminals
    for (symbol = 0; symbol < NUM_OF_TERMINALS; symbol++)
        printf(" %2d", symbol + Token_Identifier);
    // Non-Terminals
    printf(" ");
    for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
        printf("%2d ", symbol);
    printf("\n");

    // Print _
    printf("     ");
    for (symbol = 0; symbol < NUM_OF_TERMINALS + NUM_OF_NON_TERMINALS; symbol++)
        printf("___");
    printf("\n");

    // Print table
    for (state = 0; state < PARSING_TABLE_NUM_OF_STATES; state++)
    {
        printf("%2d. | ", state);

        // Action
        for (symbol = 0; symbol < NUM_OF_TERMINALS; symbol++)
        {
            if (parsing_table->action_table[state][symbol].action_type == Action_Error)
                printf(" . ");
            else if (parsing_table->action_table[state][symbol].action_type == Action_Shift)
                printf("%c%d ", 'S', parsing_table->action_table[state][symbol].state_or_rule);
            else if (parsing_table->action_table[state][symbol].action_type == Action_Reduce)
                printf("%c%d ", 'R', parsing_table->action_table[state][symbol].state_or_rule);
            else
                printf(" A ");
        }
        // Goto
        for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
            printf("%2d ", parsing_table->goto_table[state][symbol]);

        printf("\n");
    }

    printf("\n");
}
