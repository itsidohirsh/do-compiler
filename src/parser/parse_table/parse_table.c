#include <stdlib.h>
#include <stdio.h>

#include "parse_table.h"
#include "../../error_handler/error_handler.h"


Parse_Table* parse_table_create()
{
    // Create a new parsing table
    Parse_Table* parse_table = (Parse_Table*) calloc(1, sizeof(Parse_Table));
    // Check for allocation error
    if (parse_table == NULL) error_handler_report_memory_error();

    return parse_table;
}

void parse_table_destroy(Parse_Table* parse_table)
{
    free(parse_table);
}

int parse_table_get_terminal_index(Terminal_Type terminal_type)
{
    // Returning the terminal_type - 2 because the tokens enum has 2 extra tokens at the start of it,
    // Token_Error & Token_Whitespace, which we don't care about here. We want to start the indexing from 0 and not from 2.
    return terminal_type - 2;
}

void parse_table_insert_action(Parse_Table* parse_table, int state, int terminal_index, Action action)
{
    parse_table->action_table[state][terminal_index] = action;
}

void parse_table_insert_goto(Parse_Table* parse_table, int state, int non_terminal_index, int goto_state)
{
    parse_table->goto_table[state][non_terminal_index] = goto_state;
}

void parse_table_print(Parse_Table* parse_table)
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
            if (parse_table->action_table[state][symbol].action_type == Action_Error)
                printf(" .  ");
            else if (parse_table->action_table[state][symbol].action_type == Action_Shift)
                printf("%c%-2d ", 'S', parse_table->action_table[state][symbol].state_or_rule);
            else if (parse_table->action_table[state][symbol].action_type == Action_Reduce)
                printf("%c%-2d ", 'R', parse_table->action_table[state][symbol].state_or_rule);
            else
                printf(" A  ");
        }
        // Goto
        for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
            if (parse_table->goto_table[state][symbol] != 0)
                printf("%2d ", parse_table->goto_table[state][symbol]);
            else
                printf(" . ");

        printf("\n");
    }

    printf("\n");
}

void parse_table_init(Parse_Table* parse_table)
{
    // This function is dependent on the parsing table, Action & Goto, that I've 
    // created before hand according to the grammar of the language.
    // I explanined how I created it very thoroughly in the strategy chapter of my project book.


    // The current state in the parsing table
    int s;
    // The current action table entry
    Action action;


    // State 0
    s = 0;
    // - Action
    // -- Shift
    // --- `prog` -> S2
    action = (Action) { Action_Shift, 2, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Prog), action);
    // - Goto
    // -- PROG -> 1
    parse_table_insert_goto(parse_table, s, Non_Terminal_PROG, 1);


    // State 1
    s = 1;
    // - Action
    // -- Accept
    // --- EOF -> Accept
    action = (Action) { Action_Accept, 0, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Eof), action);


    // State 2
    s = 2;
    // - Action
    // -- Shift
    // --- id -> S3
    action = (Action) { Action_Shift, 3, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);


    // State 3
    s = 3;
    // - Action
    // -- Shift
    // --- `:` -> S4
    action = (Action) { Action_Shift, 4, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Colon), action);


    // State 4
    s = 4;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 5
    parse_table_insert_goto(parse_table, s, Non_Terminal_BLOCK, 5);
    // -- STMT -> 6
    parse_table_insert_goto(parse_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(parse_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(parse_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(parse_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(parse_table, s, Non_Terminal_WHILE, 11);


    // State 5
    s = 5;
    // - Action
    // -- Shift
    // --- `:)` -> S16
    action = (Action) { Action_Shift, 16, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smiley), action);


    // State 6
    s = 6;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 17
    parse_table_insert_goto(parse_table, s, Non_Terminal_BLOCK, 17);
    // -- STMT -> 6
    parse_table_insert_goto(parse_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(parse_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(parse_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(parse_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(parse_table, s, Non_Terminal_WHILE, 11);


    // State 7
    s = 7;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R2
    action = (Action) { Action_Reduce, 2, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smiley), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Else), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 8
    s = 8;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R3
    action = (Action) { Action_Reduce, 3, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 9
    s = 9;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R4
    action = (Action) { Action_Reduce, 4, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 10
    s = 10;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R5
    action = (Action) { Action_Reduce, 5, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 11
    s = 11;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R6
    action = (Action) { Action_Reduce, 6, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 12
    s = 12;
    // - Action
    // -- Shift
    // --- id -> S18
    action = (Action) { Action_Shift, 18, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);


    // State 13
    s = 13;
    // - Action
    // -- Shift
    // --- id -> S19
    action = (Action) { Action_Shift, 19, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);


    // State 14
    s = 14;
    // - Action
    // -- Shift
    // --- `(` -> S20
    action = (Action) { Action_Shift, 20, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);


    // State 15
    s = 15;
    // - Action
    // -- Shift
    // --- `(` -> S21
    action = (Action) { Action_Shift, 21, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);


    // State 16
    s = 16;
    // - Action
    // -- Reduce
    // --- EOF -> R0
    action = (Action) { Action_Reduce, 0, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Eof), action);

    // State 17
    s = 17;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R1
    action = (Action) { Action_Reduce, 1, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smiley), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Else), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 18
    s = 18;
    // - Action
    // -- Shift
    // --- `;` -> S22
    action = (Action) { Action_Shift, 22, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);


    // State 19
    s = 19;
    // - Action
    // -- Shift
    // --- `=` -> S23
    action = (Action) { Action_Shift, 23, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Assignment), action);


    // State 20
    s = 20;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 24
    parse_table_insert_goto(parse_table, s, Non_Terminal_L_LOG_E, 24);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(parse_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 21
    s = 21;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 35
    parse_table_insert_goto(parse_table, s, Non_Terminal_L_LOG_E, 35);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(parse_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 22
    s = 22;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R7
    action = (Action) { Action_Reduce, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 23
    s = 23;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 36
    parse_table_insert_goto(parse_table, s, Non_Terminal_L_LOG_E, 36);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(parse_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 24
    s = 24;
    // - Action
    // -- Shift
    // --- `)` -> S37
    action = (Action) { Action_Shift, 37, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 25
    s = 25;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    action = (Action) { Action_Shift, 39, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R14
    action = (Action) { Action_Reduce, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 26
    s = 26;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    action = (Action) { Action_Shift, 40, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R16
    action = (Action) { Action_Reduce, 16, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);


    // State 27
    s = 27;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    action = (Action) { Action_Shift, 41, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R18
    action = (Action) { Action_Reduce, 18, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);


    // State 28
    s = 28;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    action = (Action) { Action_Shift, 42, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R20
    action = (Action) { Action_Reduce, 20, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);


    // State 29
    s = 29;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R22
    action = (Action) { Action_Reduce, 22, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 30
    s = 30;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R23
    action = (Action) { Action_Reduce, 23, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 31
    s = 31;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R24
    action = (Action) { Action_Reduce, 24, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 32
    s = 32;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 43
    parse_table_insert_goto(parse_table, s, Non_Terminal_L_LOG_E, 43);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(parse_table, s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 33
    s = 33;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 44
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 44);


    // State 34
    s = 34;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 44
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 45);


    // State 35
    s = 35;
    // - Action
    // -- Shift
    // --- `)` -> S46
    action = (Action) { Action_Shift, 46, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 36
    s = 36;
    // - Action
    // -- Shift
    // --- `;` -> S47
    action = (Action) { Action_Shift, 47, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 37
    s = 37;
    // - Action
    // -- Shift
    // --- `:` -> S48
    action = (Action) { Action_Shift, 48, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Colon), action);


    // State 38
    s = 38;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- H_LOG_E -> 49
    parse_table_insert_goto(parse_table, s, Non_Terminal_H_LOG_E, 49);
    // -- BOOL_E -> 26
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 39
    s = 39;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- BOOL_E -> 50
    parse_table_insert_goto(parse_table, s, Non_Terminal_BOOL_E, 50);
    // -- E -> 27
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 40
    s = 40;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- E -> 51
    parse_table_insert_goto(parse_table, s, Non_Terminal_E, 51);
    // -- T -> 28
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 41
    s = 41;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- T -> 52
    parse_table_insert_goto(parse_table, s, Non_Terminal_T, 52);
    // -- F -> 29
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 29);


    // State 42
    s = 42;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 53
    parse_table_insert_goto(parse_table, s, Non_Terminal_F, 53);


    // State 43
    s = 43;
    // - Action
    // -- Shift
    // --- `)` -> S54
    action = (Action) { Action_Shift, 54, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 44
    s = 44;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R26
    action = (Action) { Action_Reduce, 26, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 45
    s = 45;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R27
    action = (Action) { Action_Reduce, 27, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 46
    s = 46;
    // - Action
    // -- Shift
    // --- `:` -> S55
    action = (Action) { Action_Shift, 55, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Colon), action);


    // State 47
    s = 47;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R8
    action = (Action) { Action_Reduce, 8, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 48
    s = 48;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 56
    parse_table_insert_goto(parse_table, s, Non_Terminal_BLOCK, 56);
    // -- STMT -> 6
    parse_table_insert_goto(parse_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(parse_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(parse_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(parse_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(parse_table, s, Non_Terminal_WHILE, 11);


    // State 49
    s = 49;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    action = (Action) { Action_Shift, 39, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R13
    action = (Action) { Action_Reduce, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);


    // State 50
    s = 50;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    action = (Action) { Action_Shift, 40, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R15
    action = (Action) { Action_Reduce, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);


    // State 51
    s = 51;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    action = (Action) { Action_Shift, 41, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R17
    action = (Action) { Action_Reduce, 17, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);


    // State 52
    s = 52;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    action = (Action) { Action_Shift, 42, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R19
    action = (Action) { Action_Reduce, 19, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);


    // State 53
    s = 53;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R21
    action = (Action) { Action_Reduce, 21, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 54
    s = 54;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R21
    action = (Action) { Action_Reduce, 25, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Modulu), action);


    // State 55
    s = 55;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 57
    parse_table_insert_goto(parse_table, s, Non_Terminal_BLOCK, 57);
    // -- STMT -> 6
    parse_table_insert_goto(parse_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(parse_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(parse_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(parse_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(parse_table, s, Non_Terminal_WHILE, 11);


    // State 56
    s = 56;
    // - Action
    // -- Shift
    // --- `else` -> S59
    action = (Action) { Action_Shift, 59, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Else), action);
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R11
    action = (Action) { Action_Reduce, 11, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- ELSE -> 58
    parse_table_insert_goto(parse_table, s, Non_Terminal_ELSE, 58);


    // State 57
    s = 57;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R12
    action = (Action) { Action_Reduce, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 58
    s = 58;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R9
    action = (Action) { Action_Reduce, 9, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);


    // State 59
    s = 59;
    // - Action
    // -- Shift
    // --- `:` -> S60
    action = (Action) { Action_Shift, 60, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Colon), action);


    // State 60
    s = 60;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 61
    parse_table_insert_goto(parse_table, s, Non_Terminal_BLOCK, 61);
    // -- STMT -> 6
    parse_table_insert_goto(parse_table, s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(parse_table, s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(parse_table, s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(parse_table, s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(parse_table, s, Non_Terminal_WHILE, 11);


    // State 61
    s = 61;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R10
    action = (Action) { Action_Reduce, 10, NULL };
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(parse_table, s, parse_table_get_terminal_index(Token_While), action);
}
