#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "parsing_table/parsing_table.h"
#include "../error_handler/error_handler.h"

Parser* parser_create()
{
    // Create parser
    Parser* parser = (Parser*) calloc(1, sizeof(Parser));
    // Check for allocation error
    if (parser == NULL)
    {
        parser_destroy(parser);
        error_handler_report_alloc();
    }

    // Create parser's lexer
    parser->lexer = lexer_create();

    // Creates and initializes the parser's parsing table
    parser->parsing_table = parsing_table_create();

    return parser;
}

void parser_destroy(Parser* parser)
{
    // Free the parser's lexer
    lexer_destroy(parser->lexer);

    // Free parser's parsing table
    parsing_table_destroy(parser->parsing_table);

    // Free parser's stack
    parser_stack_destroy_stack(&(parser->parser_stack));

    // Free the parser
    free(parser);
}

void parser_init(Parser* parser, char* src)
{
    // Initialize lexer
    lexer_init(parser->lexer, src);

    // Initialize parsing table
    parsing_table_init(parser->parsing_table);

    // Initialize parser's stack
    // Push the first stack entry onto the stack. For the first entry we only care about its goto_state
    // value because it's the first state we start from. In our case it's state number 0.
    parser_stack_push(&(parser->parser_stack), parser_stack_init_entry(NULL, 0));

    // Initialize production_rules array according to grammar rules of the language
    int prod_num = 0;
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_PROG, 5 };      // PROG -> prog id : BLOCK :)
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BLOCK, 2 };     // BLOCK -> STMT BLOCK
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BLOCK, 1 };     // BLOCK -> done
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> DECL
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> ASSIGN
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> IF_ELSE
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> WHILE
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_DECL, 3 };      // DECL -> data_type id
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ASSIGM, 5 };    // ASSIGN -> set id = L_LOG_E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_IF_ELSE, 7 };   // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ELSE, 3 };      // ELSE -> else : BLOCK
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ELSE, 0 };      // ELSE -> epsilon
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_WHILE, 6 };     // WHILE -> while ( L_LOG_E ) : BLOCK
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_L_LOG_E, 3 };   // L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_L_LOG_E, 1 };   // L_LOG_E -> H_LOG_E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_H_LOG_E, 3 };   // H_LOG_E -> H_LOG_E h_log_op BOOL_E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_H_LOG_E, 1 };   // H_LOG_E -> BOOL_E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BOOL_E, 3 };    // BOOL_E -> BOOL_E bool_op E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BOOL_E, 1 };    // BOOL_E -> E
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_E, 3 };         // E -> E expr_op T
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_E, 1 };         // E -> T
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_T, 3 };         // T -> T term_op F
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_T, 1 };         // T -> F
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 1 };         // F -> id
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 1 };         // F -> literal
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 3 };         // F -> ( L_LOG_E )
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 2 };         // F -> ! F
    parser->production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 2 };         // F -> - F
}

Parse_Tree_Node* parser_parse(Parser* parser, char* src)
{
    // Initialize the Stack and the Action & Goto tables
    parser_init(parser, src);

    // The current token from the source code
    Token* token;
    // The next state to go to
    int state;
    // Current action table cell
    Parsing_Table_Cell cur_action_table_cell;
    // Current tree node
    Parse_Tree_Node* cur_tree_node;
    // Current stack entry
    Parser_Stack_Entry* cur_stack_entry;

    // Input first token from the source code
    token = lexer_get_next_token(parser->lexer);

    // While not done parsing
    while (token->type != Token_Eof) // TODO: Update loop condition
    {
        char* type = token_to_str(token);
        printf("%s\n", type);
        free(type);
        token_destroy(token);

        // Get the next state from the top of the stack
        state = parser->parser_stack->goto_state;

        // Save the current cell in the action table
        cur_action_table_cell = parser->parsing_table->action_table[state][terminal_index(token->type)];

        // If Action[state, token] == Shift
        if (cur_action_table_cell.action_type == Action_Shift)
        {
            // Create a new tree node from the current token
            cur_tree_node = parse_tree_init_node(Terminal, token->type, token, NULL, 0);
            // Create a new stack entry for terminal 
            cur_stack_entry = parser_stack_init_entry(cur_tree_node, cur_action_table_cell.state_or_rule);
            // Push created stack entry on to the stack
            parser_stack_push(&(parser->parser_stack), cur_stack_entry);
            // Get next token from the source code
            token = lexer_get_next_token(parser->lexer);
        }
        // If Action[state, token] == Reduce
        else if (cur_action_table_cell.action_type == Action_Reduce)
        {
            // TODO: Implement Reduce
        }
        // If Action[state, token] == Accept
        else if (cur_action_table_cell.action_type == Action_Accept)
        {
            // TODO: Implement Accept
        }
        // If Action[state, token] == Error
        else
        {
            // TODO: Implement Error
            token = lexer_get_next_token(parser->lexer);
        }
    }
}
