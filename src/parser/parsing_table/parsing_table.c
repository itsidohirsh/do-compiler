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

int parsing_table_get_terminal_index(Terminal_Kind terminal_kind)
{
    // Returning the terminal_kind - Token_Identifier because the tokens enum has 2 extra tokens at the start of it,
    // Token_Error & Token_Whitespace, which we don't care about here. We want to start the indexing from 0 and not from 2.
    return terminal_kind - 2;
}

void parsing_table_insert_action(Parsing_Table* parsing_table, int state, int terminal_index, Action_Table_Cell cell_contents)
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
        printf("  %2d", symbol + 2);
    // Non-Terminals
    printf(" ");
    for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
        printf(" %2d", symbol);
    printf("\n");

    // Print _
    printf("     ");
    for (symbol = 0; symbol < NUM_OF_TERMINALS; symbol++)
        printf("____");
    for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
        printf("___");
    printf("_\n");

    // Print table
    for (state = 0; state < PARSING_TABLE_NUM_OF_STATES; state++)
    {
        printf("%2d. |  ", state);

        // Action
        for (symbol = 0; symbol < NUM_OF_TERMINALS; symbol++)
        {
            if (parsing_table->action_table[state][symbol].action_kind == Action_Error)
                printf(" .  ");
            else if (parsing_table->action_table[state][symbol].action_kind == Action_Shift)
                printf("%c%-2d ", 'S', parsing_table->action_table[state][symbol].state_or_rule);
            else if (parsing_table->action_table[state][symbol].action_kind == Action_Reduce)
                printf("%c%-2d ", 'R', parsing_table->action_table[state][symbol].state_or_rule);
            else
                printf(" A  ");
        }
        // Goto
        for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
            if (parsing_table->goto_table[state][symbol] != 0)
                printf("%2d ", parsing_table->goto_table[state][symbol]);
            else
                printf(" . ");

        printf("\n");
    }

    printf("\n");
}

void parsing_table_init(Parsing_Table* parsing_table)
{
    // This function is dependent on the parsing table, Action & Goto, that I 
    // created before hand according to the grammar of the language.
    // I explanined how I created it very thoroughly in the strategy chapter of my project book.


    // The current state in the parsing table
    int s;
    // The current parsing table cell contents
    Action_Table_Cell cell_contents;


    // State 0
    s = 0;
    // - Action
    // -- Shift
    // --- `prog` -> S2
    cell_contents = (Action_Table_Cell) { Action_Shift, 2, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Prog), cell_contents);
    // - Goto
    // -- PROG -> 1
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_PROG, 1);


    // State 1
    s = 1;
    // - Action
    // -- Accept
    // --- EOF -> Accept
    cell_contents = (Action_Table_Cell) { Action_Accept, 0, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Eof), cell_contents);


    // State 2
    s = 2;
    // - Action
    // -- Shift
    // --- id -> S3
    cell_contents = (Action_Table_Cell) { Action_Shift, 3, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);


    // State 3
    s = 3;
    // - Action
    // -- Shift
    // --- `:` -> S4
    cell_contents = (Action_Table_Cell) { Action_Shift, 4, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Colon), cell_contents);


    // State 4
    s = 4;
    // - Action
    // -- Shift
    // --- `done` -> S7
    cell_contents = (Action_Table_Cell) { Action_Shift, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    // --- `int` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    // --- `char` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    // --- `set` -> S13
    cell_contents = (Action_Table_Cell) { Action_Shift, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    // --- `if` -> S14
    cell_contents = (Action_Table_Cell) { Action_Shift, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    // --- `while` -> S15
    cell_contents = (Action_Table_Cell) { Action_Shift, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- BLOCK -> 5
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BLOCK, 5);
    // -- STMT -> 6
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_WHILE, 11);


    // State 5
    s = 5;
    // - Action
    // -- Shift
    // --- `:)` -> S16
    cell_contents = (Action_Table_Cell) { Action_Shift, 16, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smiley), cell_contents);


    // State 6
    s = 6;
    // - Action
    // -- Shift
    // --- `done` -> S7
    cell_contents = (Action_Table_Cell) { Action_Shift, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    // --- `int` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    // --- `char` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    // --- `set` -> S13
    cell_contents = (Action_Table_Cell) { Action_Shift, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    // --- `if` -> S14
    cell_contents = (Action_Table_Cell) { Action_Shift, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    // --- `while` -> S15
    cell_contents = (Action_Table_Cell) { Action_Shift, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- BLOCK -> 17
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BLOCK, 17);
    // -- STMT -> 6
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_WHILE, 11);


    // State 7
    s = 7;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R2
    cell_contents = (Action_Table_Cell) { Action_Reduce, 2, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smiley), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Else), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 8
    s = 8;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R3
    cell_contents = (Action_Table_Cell) { Action_Reduce, 3, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 9
    s = 9;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R4
    cell_contents = (Action_Table_Cell) { Action_Reduce, 4, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 10
    s = 10;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R5
    cell_contents = (Action_Table_Cell) { Action_Reduce, 5, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 11
    s = 11;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R6
    cell_contents = (Action_Table_Cell) { Action_Reduce, 6, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 12
    s = 12;
    // - Action
    // -- Shift
    // --- id -> S18
    cell_contents = (Action_Table_Cell) { Action_Shift, 18, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);


    // State 13
    s = 13;
    // - Action
    // -- Shift
    // --- id -> S19
    cell_contents = (Action_Table_Cell) { Action_Shift, 19, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);


    // State 14
    s = 14;
    // - Action
    // -- Shift
    // --- `(` -> S20
    cell_contents = (Action_Table_Cell) { Action_Shift, 20, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);


    // State 15
    s = 15;
    // - Action
    // -- Shift
    // --- `(` -> S21
    cell_contents = (Action_Table_Cell) { Action_Shift, 21, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);


    // State 16
    s = 16;
    // - Action
    // -- Reduce
    // --- EOF -> R0
    cell_contents = (Action_Table_Cell) { Action_Reduce, 0, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Eof), cell_contents);

    // State 17
    s = 17;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R1
    cell_contents = (Action_Table_Cell) { Action_Reduce, 1, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smiley), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Else), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 18
    s = 18;
    // - Action
    // -- Shift
    // --- `;` -> S22
    cell_contents = (Action_Table_Cell) { Action_Shift, 22, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);


    // State 19
    s = 19;
    // - Action
    // -- Shift
    // --- `=` -> S23
    cell_contents = (Action_Table_Cell) { Action_Shift, 23, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Assignment), cell_contents);


    // State 20
    s = 20;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- L_LOG_E -> 24
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_L_LOG_E, 24);
    // -- H_LOG_E -> 25
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 21
    s = 21;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- L_LOG_E -> 35
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_L_LOG_E, 35);
    // -- H_LOG_E -> 25
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 22
    s = 22;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R7
    cell_contents = (Action_Table_Cell) { Action_Reduce, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 23
    s = 23;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- L_LOG_E -> 36
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_L_LOG_E, 36);
    // -- H_LOG_E -> 25
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 24
    s = 24;
    // - Action
    // -- Shift
    // --- `)` -> S37
    cell_contents = (Action_Table_Cell) { Action_Shift, 37, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    // --- `||` -> S38
    cell_contents = (Action_Table_Cell) { Action_Shift, 38, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 25
    s = 25;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    cell_contents = (Action_Table_Cell) { Action_Shift, 39, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R14
    cell_contents = (Action_Table_Cell) { Action_Reduce, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 26
    s = 26;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    cell_contents = (Action_Table_Cell) { Action_Shift, 40, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R16
    cell_contents = (Action_Table_Cell) { Action_Reduce, 16, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);


    // State 27
    s = 27;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    cell_contents = (Action_Table_Cell) { Action_Shift, 41, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R18
    cell_contents = (Action_Table_Cell) { Action_Reduce, 18, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);


    // State 28
    s = 28;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    cell_contents = (Action_Table_Cell) { Action_Shift, 42, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R20
    cell_contents = (Action_Table_Cell) { Action_Reduce, 20, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);


    // State 29
    s = 29;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R22
    cell_contents = (Action_Table_Cell) { Action_Reduce, 22, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 30
    s = 30;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R23
    cell_contents = (Action_Table_Cell) { Action_Reduce, 23, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 31
    s = 31;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R24
    cell_contents = (Action_Table_Cell) { Action_Reduce, 24, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 32
    s = 32;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- L_LOG_E -> 43
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_L_LOG_E, 43);
    // -- H_LOG_E -> 25
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 33
    s = 33;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- F -> 44
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 44);


    // State 34
    s = 34;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- F -> 44
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 45);


    // State 35
    s = 35;
    // - Action
    // -- Shift
    // --- `)` -> S46
    cell_contents = (Action_Table_Cell) { Action_Shift, 46, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    // --- `||` -> S38
    cell_contents = (Action_Table_Cell) { Action_Shift, 38, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 36
    s = 36;
    // - Action
    // -- Shift
    // --- `;` -> S47
    cell_contents = (Action_Table_Cell) { Action_Shift, 47, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    // --- `||` -> S38
    cell_contents = (Action_Table_Cell) { Action_Shift, 38, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 37
    s = 37;
    // - Action
    // -- Shift
    // --- `:` -> S48
    cell_contents = (Action_Table_Cell) { Action_Shift, 48, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Colon), cell_contents);


    // State 38
    s = 38;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- H_LOG_E -> 49
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_H_LOG_E, 49);
    // -- BOOL_E -> 26
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 39
    s = 39;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- BOOL_E -> 50
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BOOL_E, 50);
    // -- E -> 27
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 40
    s = 40;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- E -> 51
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_E, 51);
    // -- T -> 28
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 41
    s = 41;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- T -> 52
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_T, 52);
    // -- F -> 29
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 29);


    // State 42
    s = 42;
    // - Action
    // -- Shift
    // --- id -> S30
    cell_contents = (Action_Table_Cell) { Action_Shift, 30, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Identifier), cell_contents);
    // --- `(` -> S32
    cell_contents = (Action_Table_Cell) { Action_Shift, 32, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Open_Paren), cell_contents);
    // --- `-` -> S34
    cell_contents = (Action_Table_Cell) { Action_Shift, 34, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // --- number -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Number), cell_contents);
    // --- character -> S31
    cell_contents = (Action_Table_Cell) { Action_Shift, 31, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Character), cell_contents);
    // --- `!` -> S33
    cell_contents = (Action_Table_Cell) { Action_Shift, 33, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not), cell_contents);

    // - Goto
    // -- F -> 53
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_F, 53);


    // State 43
    s = 43;
    // - Action
    // -- Shift
    // --- `)` -> S54
    cell_contents = (Action_Table_Cell) { Action_Shift, 54, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    // --- `||` -> S38
    cell_contents = (Action_Table_Cell) { Action_Shift, 38, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 44
    s = 44;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R26
    cell_contents = (Action_Table_Cell) { Action_Reduce, 26, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 45
    s = 45;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R27
    cell_contents = (Action_Table_Cell) { Action_Reduce, 27, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 46
    s = 46;
    // - Action
    // -- Shift
    // --- `:` -> S55
    cell_contents = (Action_Table_Cell) { Action_Shift, 55, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Colon), cell_contents);


    // State 47
    s = 47;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R8
    cell_contents = (Action_Table_Cell) { Action_Reduce, 8, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 48
    s = 48;
    // - Action
    // -- Shift
    // --- `done` -> S7
    cell_contents = (Action_Table_Cell) { Action_Shift, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    // --- `int` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    // --- `char` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    // --- `set` -> S13
    cell_contents = (Action_Table_Cell) { Action_Shift, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    // --- `if` -> S14
    cell_contents = (Action_Table_Cell) { Action_Shift, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    // --- `while` -> S15
    cell_contents = (Action_Table_Cell) { Action_Shift, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- BLOCK -> 56
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BLOCK, 56);
    // -- STMT -> 6
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_WHILE, 11);


    // State 49
    s = 49;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    cell_contents = (Action_Table_Cell) { Action_Shift, 39, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R13
    cell_contents = (Action_Table_Cell) { Action_Reduce, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);


    // State 50
    s = 50;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    cell_contents = (Action_Table_Cell) { Action_Shift, 40, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R15
    cell_contents = (Action_Table_Cell) { Action_Reduce, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);


    // State 51
    s = 51;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    cell_contents = (Action_Table_Cell) { Action_Shift, 41, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R17
    cell_contents = (Action_Table_Cell) { Action_Reduce, 17, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);


    // State 52
    s = 52;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    cell_contents = (Action_Table_Cell) { Action_Shift, 42, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R19
    cell_contents = (Action_Table_Cell) { Action_Reduce, 19, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);


    // State 53
    s = 53;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R21
    cell_contents = (Action_Table_Cell) { Action_Reduce, 21, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 54
    s = 54;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R21
    cell_contents = (Action_Table_Cell) { Action_Reduce, 25, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Semi_Colon), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Close_Paren), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Or), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_And), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Not_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Bigger_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Smaller_Equal), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Plus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Minus), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Multiply), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Divide), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Modulu), cell_contents);


    // State 55
    s = 55;
    // - Action
    // -- Shift
    // --- `done` -> S7
    cell_contents = (Action_Table_Cell) { Action_Shift, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    // --- `int` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    // --- `char` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    // --- `set` -> S13
    cell_contents = (Action_Table_Cell) { Action_Shift, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    // --- `if` -> S14
    cell_contents = (Action_Table_Cell) { Action_Shift, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    // --- `while` -> S15
    cell_contents = (Action_Table_Cell) { Action_Shift, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- BLOCK -> 57
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BLOCK, 57);
    // -- STMT -> 6
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_WHILE, 11);


    // State 56
    s = 56;
    // - Action
    // -- Shift
    // --- `else` -> S59
    cell_contents = (Action_Table_Cell) { Action_Shift, 59, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Else), cell_contents);
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R11
    cell_contents = (Action_Table_Cell) { Action_Reduce, 11, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- ELSE -> 58
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ELSE, 58);


    // State 57
    s = 57;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R12
    cell_contents = (Action_Table_Cell) { Action_Reduce, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 58
    s = 58;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R9
    cell_contents = (Action_Table_Cell) { Action_Reduce, 9, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);


    // State 59
    s = 59;
    // - Action
    // -- Shift
    // --- `:` -> S60
    cell_contents = (Action_Table_Cell) { Action_Shift, 60, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Colon), cell_contents);


    // State 60
    s = 60;
    // - Action
    // -- Shift
    // --- `done` -> S7
    cell_contents = (Action_Table_Cell) { Action_Shift, 7, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    // --- `int` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    // --- `char` -> S12
    cell_contents = (Action_Table_Cell) { Action_Shift, 12, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    // --- `set` -> S13
    cell_contents = (Action_Table_Cell) { Action_Shift, 13, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    // --- `if` -> S14
    cell_contents = (Action_Table_Cell) { Action_Shift, 14, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    // --- `while` -> S15
    cell_contents = (Action_Table_Cell) { Action_Shift, 15, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);

    // - Goto
    // -- BLOCK -> 61
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_BLOCK, 61);
    // -- STMT -> 6
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parsing_table_insert_goto(parsing_table, s, Non_Terminal_WHILE, 11);


    // State 61
    s = 61;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R10
    cell_contents = (Action_Table_Cell) { Action_Reduce, 10, NULL };
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Done), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Int), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Char), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_Set), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_If), cell_contents);
    parsing_table_insert_action(parsing_table, s, parsing_table_get_terminal_index(Token_While), cell_contents);
}
