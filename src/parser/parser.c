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

    // TODO: Make all destroy functions double pointer to makae the pointer NULL to prevent free() not NULL pointer
}

void parser_init_production_rules(Production_Rule* production_rules)
{
    // Holds the number of the current production rule
    int prod_num = 0;
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_PROG, 5 };      // PROG -> prog id : BLOCK :)
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BLOCK, 2 };     // BLOCK -> STMT BLOCK
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BLOCK, 1 };     // BLOCK -> done
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> DECL
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> ASSIGN
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> IF_ELSE
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_STMT, 1 };      // STMT -> WHILE
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_DECL, 3 };      // DECL -> data_type id
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ASSIGN, 5 };    // ASSIGN -> set id = L_LOG_E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_IF_ELSE, 7 };   // IF_ELSE -> if ( L_LOG_E ) : BLOCK ELSE
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ELSE, 3 };      // ELSE -> else : BLOCK
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_ELSE, 0 };      // ELSE -> epsilon
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_WHILE, 6 };     // WHILE -> while ( L_LOG_E ) : BLOCK
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_L_LOG_E, 3 };   // L_LOG_E -> L_LOG_E l_log_op H_LOG_E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_L_LOG_E, 1 };   // L_LOG_E -> H_LOG_E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_H_LOG_E, 3 };   // H_LOG_E -> H_LOG_E h_log_op BOOL_E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_H_LOG_E, 1 };   // H_LOG_E -> BOOL_E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BOOL_E, 3 };    // BOOL_E -> BOOL_E bool_op E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_BOOL_E, 1 };    // BOOL_E -> E
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_E, 3 };         // E -> E expr_op T
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_E, 1 };         // E -> T
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_T, 3 };         // T -> T term_op F
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_T, 1 };         // T -> F
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 1 };         // F -> id
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 1 };         // F -> literal
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 3 };         // F -> ( L_LOG_E )
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 2 };         // F -> ! F
    production_rules[prod_num++] = (Production_Rule) { Non_Terminal_F, 2 };         // F -> - F
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
    parser_init_production_rules(parser->production_rules);
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
    Action_Table_Cell cur_action_table_cell;
    // Current tree node
    Parse_Tree_Node* cur_tree_node;
    // Current tree node array, the current node's children
    Parse_Tree_Node** cur_children;
    // Current stack entry
    Parser_Stack_Entry* cur_stack_entry;
    // Current production rule
    Production_Rule cur_production_rule;
    // Iterator
    int i;

    // Input first token from the source code
    token = lexer_get_next_token(parser->lexer);

    // While not done parsing. While true because we stop only when we accept or error. In both cases we exit correctly.
    while (true)
    {
        // Get the next state from the top of the stack
        state = parser->parser_stack->goto_state;

        // Save the current cell in the action table
        cur_action_table_cell = parser->parsing_table->action_table[state][parsing_table_get_terminal_index(token->token_kind)];

        // If Action[state, token] == Shift
        if (cur_action_table_cell.action_kind == Action_Shift)
        {
            // Create a new tree node from the current token
            cur_tree_node = parse_tree_init_node(Terminal, token->token_kind, token, NULL, 0);
            // Create a new stack entry for terminal 
            cur_stack_entry = parser_stack_init_entry(cur_tree_node, cur_action_table_cell.state_or_rule);
            // Push created stack entry onto the stack
            parser_stack_push(&(parser->parser_stack), cur_stack_entry);
            // Get next token from the source code
            token = lexer_get_next_token(parser->lexer);
        }
        // If Action[state, token] == Reduce
        else if (cur_action_table_cell.action_kind == Action_Reduce)
        {
            // Get the current production rule
            cur_production_rule = parser->production_rules[cur_action_table_cell.state_or_rule];
            // Create an array of the size of number of symbols on the RHS of the production rule that we reduce by
            cur_children = (Parse_Tree_Node**) calloc(cur_production_rule.rule_length, sizeof(Parse_Tree_Node*));
            // Check allocation error
            if (cur_children == NULL)
            {
                // Destroy parser
                parser_destroy(parser);
                error_handler_report_alloc();
            }
            // Pop Length(Production rule) from the stack, and put the nodes of each entry as a child in the array
            // From left of array to the right because the production rule is "reversed" in the stack
            for (i = cur_production_rule.rule_length - 1; i >= 0; i--)
            {
                // Pop the top entry from the stack
                cur_stack_entry = parser_stack_pop(&(parser->parser_stack));
                // Place its tree in the children array
                cur_children[i] = cur_stack_entry->tree_node;
                // Disconnect tree from the stack entry
                cur_stack_entry->tree_node = NULL;
                // Destroy that entry
                parser_stack_destroy_entry(cur_stack_entry);
            }
            // Create a new tree node from the non-terminal on the LHS of the production rule we reduce by
            cur_tree_node = parse_tree_init_node(Non_Terminal, cur_production_rule.non_terminal_kind, NULL, cur_children, cur_production_rule.rule_length);
            // Create a new stack entry using the created tree node and Goto table
            cur_stack_entry = parser_stack_init_entry(cur_tree_node, parser->parsing_table->goto_table[parser->parser_stack->goto_state][cur_production_rule.non_terminal_kind]);
            // Push created entry onto the stack
            parser_stack_push(&(parser->parser_stack), cur_stack_entry);
        }
        // If Action[state, token] == Accept
        else if (cur_action_table_cell.action_kind == Action_Accept)
        {
            // When reached an Accept, the stack only has one entry other than the start entry, which contains the parse tree
            cur_tree_node = parser->parser_stack->tree_node;
            // Disconnect tree node from entry so it won't be destroyed
            parser->parser_stack->tree_node = NULL;
            // Destroy parser
            parser_destroy(parser);
            // Return parsing tree
            return cur_tree_node;
        }
        // If Action[state, token] == Error
        else
        {
            // Save line before distroying parser
            int line = parser->lexer->line;
            // When reached an Error, Destroy the parser, output error and exit
            parser_destroy(parser);
            // Report on an error and exit
            error_handler_report(line, "Parser: Unexpected token `%s`", 1, (void* []) { (void*) token->value });
        }
    }
}
