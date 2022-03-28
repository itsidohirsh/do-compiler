#include <stdlib.h>
#include <stdio.h>

#include "../../global.h"

#include "parse_table.h"
#include "../../general/general.h"
#include "../../error_handler/error_handler.h"
#include "../../semantic/semantic.h"


void parse_table_create()
{
    // Create a new parsing table
    compiler.parser->parse_table = (Parse_Table*) calloc(1, sizeof(Parse_Table));
    if (compiler.parser->parse_table == NULL) exit_memory_error(__FILE__, __LINE__);
}

void parse_table_destroy()
{
    // check for NULL pointer
    if (compiler.parser != NULL)
    {
        free(compiler.parser->parse_table);
        compiler.parser->parse_table = NULL;
    }
}

int parse_table_get_terminal_index(Terminal_Type terminal_type)
{
    // Returning the terminal_type - 2 because the tokens enum has 2 extra tokens at the start of it,
    // Token_Error & Token_Whitespace, which we don't care about here. We want to start the indexing from 0 and not from 2.
    return terminal_type - 2;
}

void parse_table_insert_action(int state, int terminal_index, Action action)
{
    compiler.parser->parse_table->action_table[state][terminal_index] = action;
}

void parse_table_insert_goto(int state, int non_terminal_index, int goto_state)
{
    compiler.parser->parse_table->goto_table[state][non_terminal_index] = goto_state;
}

void parse_table_print()
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
            if (compiler.parser->parse_table->action_table[state][symbol].action_type == Action_Error)
                printf(" .  ");
            else if (compiler.parser->parse_table->action_table[state][symbol].action_type == Action_Shift)
                printf("%c%-2d ", 'S', compiler.parser->parse_table->action_table[state][symbol].state_or_rule);
            else if (compiler.parser->parse_table->action_table[state][symbol].action_type == Action_Reduce)
                printf("%c%-2d ", 'R', compiler.parser->parse_table->action_table[state][symbol].state_or_rule);
            else
                printf(" A  ");
        }
        // Goto
        for (symbol = 0; symbol < NUM_OF_NON_TERMINALS; symbol++)
            if (compiler.parser->parse_table->goto_table[state][symbol] != 0)
                printf("%2d ", compiler.parser->parse_table->goto_table[state][symbol]);
            else
                printf(" . ");

        printf("\n");
    }

    printf("\n");
}

void parse_table_init()
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
    action = (Action) { Action_Shift, 2, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Prog), action);
    // - Goto
    // -- PROG -> 1
    parse_table_insert_goto(s, Non_Terminal_PROG, 1);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_prog;


    // State 1
    s = 1;
    // - Action
    // -- Accept
    // --- EOF -> Accept
    action = (Action) { Action_Accept, 0, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Eof), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_eof;


    // State 2
    s = 2;
    // - Action
    // -- Shift
    // --- id -> S3
    action = (Action) { Action_Shift, 3, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_prog_name;


    // State 3
    s = 3;
    // - Action
    // -- Shift
    // --- `:` -> S4
    // ---- Here I'm not using semantic_enter_block() when encountering : because here is the enter to the global
    // ---- scope, which is already created when intializing the scope tree.
    action = (Action) { Action_Shift, 4, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Colon), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_colon_state_3;


    // State 4
    s = 4;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 5
    parse_table_insert_goto(s, Non_Terminal_BLOCK, 5);
    // -- STMT -> 6
    parse_table_insert_goto(s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(s, Non_Terminal_WHILE, 11);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 5
    s = 5;
    // - Action
    // -- Shift
    // --- `:)` -> S16
    action = (Action) { Action_Shift, 16, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smiley), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_smiley;


    // State 6
    s = 6;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 17
    parse_table_insert_goto(s, Non_Terminal_BLOCK, 17);
    // -- STMT -> 6
    parse_table_insert_goto(s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(s, Non_Terminal_WHILE, 11);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 7
    s = 7;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R2
    // ---- Using semantic_exit_block() because when we reduce by rule no. 2 we know we've exited a scope.
    // ---- Rule no. 2: BLOCK -> done
    action = (Action) { Action_Reduce, 2, semantic_exit_block, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smiley), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Else), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_7_group;


    // State 8
    s = 8;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R3
    action = (Action) { Action_Reduce, 3, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 9
    s = 9;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R4
    action = (Action) { Action_Reduce, 4, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 10
    s = 10;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R5
    action = (Action) { Action_Reduce, 5, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 11
    s = 11;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R6
    action = (Action) { Action_Reduce, 6, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 12
    s = 12;
    // - Action
    // -- Shift
    // --- id -> S18
    action = (Action) { Action_Shift, 18, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_identifier;


    // State 13
    s = 13;
    // - Action
    // -- Shift
    // --- id -> S19
    action = (Action) { Action_Shift, 19, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_identifier;


    // State 14
    s = 14;
    // - Action
    // -- Shift
    // --- `(` -> S20
    action = (Action) { Action_Shift, 20, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_open_paren;


    // State 15
    s = 15;
    // - Action
    // -- Shift
    // --- `(` -> S21
    action = (Action) { Action_Shift, 21, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_open_paren;


    // State 16
    s = 16;
    // - Action
    // -- Reduce
    // --- EOF -> R0
    action = (Action) { Action_Reduce, 0, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Eof), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_eof;


    // State 17
    s = 17;
    // - Action
    // -- Reduce
    // --- for (`:)`, `done`, `int`, `char`, `set`, `if`, `else`, `while`) -> R1
    action = (Action) { Action_Reduce, 1, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smiley), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Else), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_7_group;


    // State 18
    s = 18;
    // - Action
    // -- Shift
    // --- `;` -> S22
    action = (Action) { Action_Shift, 22, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_semi_colon_state_18;


    // State 19
    s = 19;
    // - Action
    // -- Shift
    // --- `=` -> S23
    action = (Action) { Action_Shift, 23, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Assignment), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_assign;


    // State 20
    s = 20;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 24
    parse_table_insert_goto(s, Non_Terminal_L_LOG_E, 24);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 21
    s = 21;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 35
    parse_table_insert_goto(s, Non_Terminal_L_LOG_E, 35);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 22
    s = 22;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R7
    // ---- Using semantic_decl() because after reducing by rule no. 7, we must check for unique identifier.
    // ---- Rule no. 7: DECL -> data_type id ;
    action = (Action) { Action_Reduce, 7, semantic_decl, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 23
    s = 23;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 36
    parse_table_insert_goto(s, Non_Terminal_L_LOG_E, 36);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 24
    s = 24;
    // - Action
    // -- Shift
    // --- `)` -> S37
    action = (Action) { Action_Shift, 37, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_24_group;


    // State 25
    s = 25;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    action = (Action) { Action_Shift, 39, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R14
    // ---- Using semantic_set_type() because after reducing by rule no. 14, we need to set the LHS non-terminal's type to the RHS type
    // ---- Rule no. 14: L_LOG_E -> H_LOG_E
    action = (Action) { Action_Reduce, 14, semantic_set_type, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 26
    s = 26;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    action = (Action) { Action_Shift, 40, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R16
    // ---- Using semantic_set_type() because after reducing by rule no. 16, we need to set the LHS non-terminal's type to the RHS type
    // ---- Rule no. 16: H_LOG_E -> BOOL_E
    action = (Action) { Action_Reduce, 16, semantic_set_type, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 27
    s = 27;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    action = (Action) { Action_Shift, 41, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R18
    // ---- Using semantic_set_type() because after reducing by rule no. 18, we need to set the LHS non-terminal's type to the RHS type
    // ---- Rule no. 18: BOOL_E -> E
    action = (Action) { Action_Reduce, 18, semantic_set_type, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 28
    s = 28;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    action = (Action) { Action_Shift, 42, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R20
    // ---- Using semantic_set_type() because after reducing by rule no. 20, we need to set the LHS non-terminal's type to the RHS type
    // ---- Rule no. 20: E -> T
    action = (Action) { Action_Reduce, 20, semantic_set_type, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 29
    s = 29;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R22
    // ---- Using semantic_set_type() because after reducing by rule no. 22, we need to set the LHS non-terminal's type to the RHS type
    // ---- Rule no. 22: T -> F
    action = (Action) { Action_Reduce, 22, semantic_set_type, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 30
    s = 30;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R23
    // ---- Using semantic_F_to_id() because after reducing by rule no. 23, we must check that id exists and set type accordingly.
    // ---- Rule no. 23: F -> id
    action = (Action) { Action_Reduce, 23, semantic_F_to_id, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 31
    s = 31;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R24
    // ---- Using semantic_F_to_literal() because after reducing by rule no. 24, we need to set type accordingly.
    // ---- Rule no. 24: F -> literal
    action = (Action) { Action_Reduce, 24, semantic_F_to_literal, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 32
    s = 32;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- L_LOG_E -> 43
    parse_table_insert_goto(s, Non_Terminal_L_LOG_E, 43);
    // -- H_LOG_E -> 25
    parse_table_insert_goto(s, Non_Terminal_H_LOG_E, 25);
    // -- BOOL_E -> 26
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 33
    s = 33;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 44
    parse_table_insert_goto(s, Non_Terminal_F, 44);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 34
    s = 34;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 44
    parse_table_insert_goto(s, Non_Terminal_F, 45);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 35
    s = 35;
    // - Action
    // -- Shift
    // --- `)` -> S46
    action = (Action) { Action_Shift, 46, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_24_group;


    // State 36
    s = 36;
    // - Action
    // -- Shift
    // --- `;` -> S47
    action = (Action) { Action_Shift, 47, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_36_group;


    // State 37
    s = 37;
    // - Action
    // -- Shift
    // --- `:` -> S48
    // ---- Using semantic_enter_block() because every time we encounter : we know we've entered a new block.
    action = (Action) { Action_Shift, 48, semantic_enter_block, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Colon), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_colon_state_37;


    // State 38
    s = 38;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- H_LOG_E -> 49
    parse_table_insert_goto(s, Non_Terminal_H_LOG_E, 49);
    // -- BOOL_E -> 26
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 26);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 39
    s = 39;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- BOOL_E -> 50
    parse_table_insert_goto(s, Non_Terminal_BOOL_E, 50);
    // -- E -> 27
    parse_table_insert_goto(s, Non_Terminal_E, 27);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 40
    s = 40;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- E -> 51
    parse_table_insert_goto(s, Non_Terminal_E, 51);
    // -- T -> 28
    parse_table_insert_goto(s, Non_Terminal_T, 28);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 41
    s = 41;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- T -> 52
    parse_table_insert_goto(s, Non_Terminal_T, 52);
    // -- F -> 29
    parse_table_insert_goto(s, Non_Terminal_F, 29);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 42
    s = 42;
    // - Action
    // -- Shift
    // --- id -> S30
    action = (Action) { Action_Shift, 30, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Identifier), action);
    // --- `(` -> S32
    action = (Action) { Action_Shift, 32, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Open_Paren), action);
    // --- `-` -> S34
    action = (Action) { Action_Shift, 34, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // --- number -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Number), action);
    // --- character -> S31
    action = (Action) { Action_Shift, 31, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Character), action);
    // --- `!` -> S33
    action = (Action) { Action_Shift, 33, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not), action);

    // - Goto
    // -- F -> 53
    parse_table_insert_goto(s, Non_Terminal_F, 53);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_20_group;


    // State 43
    s = 43;
    // - Action
    // -- Shift
    // --- `)` -> S54
    action = (Action) { Action_Shift, 54, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    // --- `||` -> S38
    action = (Action) { Action_Shift, 38, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_24_group;


    // State 44
    s = 44;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R26
    // ---- Using semantic_F_to_unary_op_F() because after reducing by rule no. 26, we need to set type accordingly.
    // ---- Rule no. 26: F -> ! F
    action = (Action) { Action_Reduce, 26, semantic_F_to_unary_op_F, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 45
    s = 45;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R27
    // ---- Using semantic_F_to_unary_op_F() because after reducing by rule no. 26, we need to set type accordingly.
    // ---- Rule no. 27: F -> - F
    action = (Action) { Action_Reduce, 27, semantic_F_to_unary_op_F, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 46
    s = 46;
    // - Action
    // -- Shift
    // --- `:` -> S55
    // ---- Using semantic_enter_block() because every time we encounter : we know we've entered a new block.
    action = (Action) { Action_Shift, 55, semantic_enter_block, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Colon), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_colon_state_46;


    // State 47
    s = 47;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R8
    // ---- Using semantic_assign() because after reducing by rule no. 8, we must check for:
    // ---- 1. Existing identifier
    // ---- 2. Matching types
    // ---- Rule no. 7: DECL -> data_type id ;
    action = (Action) { Action_Reduce, 8, semantic_assign, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 48
    s = 48;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 56
    parse_table_insert_goto(s, Non_Terminal_BLOCK, 56);
    // -- STMT -> 6
    parse_table_insert_goto(s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(s, Non_Terminal_WHILE, 11);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 49
    s = 49;
    // - Action
    // -- Shift
    // --- `&&` -> S39
    action = (Action) { Action_Shift, 39, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`) -> R13
    // ---- Using semantic_type_check() because after reducing by rule no. 13, we must check matching types of the operands.
    // ---- Rule no. 13: L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    action = (Action) { Action_Reduce, 13, semantic_type_check, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 50
    s = 50;
    // - Action
    // -- Shift
    // --- for (`==`, `!=`, `>`, `>=`, `<`, `<=`) -> S40
    action = (Action) { Action_Shift, 40, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`) -> R15
    // ---- Using semantic_type_check() because after reducing by rule no. 15, we must check matching types of the operands.
    // ---- Rule no. 15: H_LOG_E -> H_LOG_E h_log_op BOOL_E
    action = (Action) { Action_Reduce, 15, semantic_type_check, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 51
    s = 51;
    // - Action
    // -- Shift
    // --- for (`+`, `-`) -> S41
    action = (Action) { Action_Shift, 41, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`) -> R17
    // ---- Using semantic_type_check() because after reducing by rule no. 17, we must check matching types of the operands.
    // ---- Rule no. 17: BOOL_E -> BOOL_E bool_op E
    action = (Action) { Action_Reduce, 17, semantic_type_check, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 52
    s = 52;
    // - Action
    // -- Shift
    // --- for (`*`, `/`, `%`) -> S42
    action = (Action) { Action_Shift, 42, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`) -> R19
    // ---- Using semantic_type_check() because after reducing by rule no. 19, we must check matching types of the operands.
    // ---- Rule no. 19: E -> E expr_op T
    action = (Action) { Action_Reduce, 19, semantic_type_check, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 53
    s = 53;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R21
    // ---- Using semantic_type_check() because after reducing by rule no. 21, we must check matching types of the operands.
    // ---- Rule no. 21: T -> T term_op F
    action = (Action) { Action_Reduce, 21, semantic_type_check, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 54
    s = 54;
    // - Action
    // -- Reduce
    // --- for (`;`, `)`, `||`, `&&`, `==`, `!=`, `>`, `>=`, `<`, `<=`, `+`, `-`, `*`, `/`, `%`) -> R25
    // ---- Using semantic_F_to_L_LOG_E() because after reducing by rule no. 25, we need to set type accordingly.
    // ---- Rule no. 25: F -> ( L_LOG_E )
    action = (Action) { Action_Reduce, 25, semantic_F_to_L_LOG_E, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Semi_Colon), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Close_Paren), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Or), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_And), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Not_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Bigger_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Smaller_Equal), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Plus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Minus), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Multiply), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Divide), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Modulu), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_25_group;


    // State 55
    s = 55;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 57
    parse_table_insert_goto(s, Non_Terminal_BLOCK, 57);
    // -- STMT -> 6
    parse_table_insert_goto(s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(s, Non_Terminal_WHILE, 11);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 56
    s = 56;
    // - Action
    // -- Shift
    // --- `else` -> S59
    action = (Action) { Action_Shift, 59, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Else), action);
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R11
    action = (Action) { Action_Reduce, 11, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- ELSE -> 58
    parse_table_insert_goto(s, Non_Terminal_ELSE, 58);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_56_group;


    // State 57
    s = 57;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R12
    action = (Action) { Action_Reduce, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 58
    s = 58;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R9
    action = (Action) { Action_Reduce, 9, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 59
    s = 59;
    // - Action
    // -- Shift
    // --- `:` -> S60
    // ---- Using semantic_enter_block() because every time we encounter : we know we've entered a new block.
    action = (Action) { Action_Shift, 60, semantic_enter_block, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Colon), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_colon_state_59;


    // State 60
    s = 60;
    // - Action
    // -- Shift
    // --- `done` -> S7
    action = (Action) { Action_Shift, 7, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    // --- `int` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    // --- `char` -> S12
    action = (Action) { Action_Shift, 12, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    // --- `set` -> S13
    action = (Action) { Action_Shift, 13, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    // --- `if` -> S14
    action = (Action) { Action_Shift, 14, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    // --- `while` -> S15
    action = (Action) { Action_Shift, 15, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Goto
    // -- BLOCK -> 61
    parse_table_insert_goto(s, Non_Terminal_BLOCK, 61);
    // -- STMT -> 6
    parse_table_insert_goto(s, Non_Terminal_STMT, 6);
    // -- DECL -> 8
    parse_table_insert_goto(s, Non_Terminal_DECL, 8);
    // -- ASSIGN -> 9
    parse_table_insert_goto(s, Non_Terminal_ASSIGN, 9);
    // -- IF_ELSE -> 10
    parse_table_insert_goto(s, Non_Terminal_IF_ELSE, 10);
    // -- WHILE -> 11
    parse_table_insert_goto(s, Non_Terminal_WHILE, 11);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;


    // State 61
    s = 61;
    // - Action
    // -- Reduce
    // --- for (`done`, `int`, `char`, `set`, `if`, `while`) -> R10
    action = (Action) { Action_Reduce, 10, NULL, NULL };
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Done), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Int), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Char), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_Set), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_If), action);
    parse_table_insert_action(s, parse_table_get_terminal_index(Token_While), action);

    // - Error function
    for (int i = 0; i < NUM_OF_TERMINALS; i++)
        compiler.parser->parse_table->action_table[s][i].error_func = error_handler_report_expected_state_4_group;
}
