#include <stdlib.h>

#include "parser.h"
#include "../error_handler/error_handler.h"


Parser* parser_create()
{
    // Create parser
    Parser* parser = (Parser*) calloc(1, sizeof(Parser));
    // Check for allocation error
    if (parser == NULL)
    {
        parser_destroy(&parser);
        error_handler_report_memory_error();
    }

    // Create parser's lexer
    parser->lexer = lexer_create();

    // Creates and initializes the parser's parsing table
    parser->parse_table = parse_table_create();

    return parser;
}

void parser_destroy(Parser** parser)
{
    // check for NULL pointer
    if (*parser != NULL)
    {
        // Free the parser's lexer
        lexer_destroy(&((*parser)->lexer));

        // Free parser's parsing table
        parse_table_destroy(&((*parser)->parse_table));

        // Free parser's stack
        parse_stack_destroy_stack(&((*parser)->parse_stack));

        // Free the parser
        free(*parser);
        *parser = NULL;
    }
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
    parse_table_init(parser->parse_table);

    // Initialize parser's stack
    // Push the first stack entry onto the stack. For the first entry we only care about its goto_state
    // value because it's the first state we start from. In our case it's state number 0.
    parse_stack_push(&(parser->parse_stack), parse_stack_init_entry(NULL, 0));

    // Initialize production_rules array according to grammar rules of the language
    parser_init_production_rules(parser->production_rules);
}

Parse_Tree_Node* parser_parse(Parser* parser, char* src)
{
    // Initializes the parser. Lexer, parse table, stack, and production rules
    parser_init(parser, src);

    // The current token from the source code
    Token* token;
    // The next state to go to
    int state;
    // Current action table cell
    Action action;
    // Current tree node
    Parse_Tree_Node* tree_node;
    // Current node's children array
    Parse_Tree_Node** children;
    // Current stack entry
    Parse_Stack_Entry* stack_entry;
    // Current production rule
    Production_Rule production_rule;
    // Iterator
    int i;

    // Input first token from the source code
    token = lexer_get_next_token(parser->lexer);

    // While not done parsing. We'll be done parsing by an Accept or Error
    while (true)
    {
        // Get the next state from the top of the stack
        state = parser->parse_stack->goto_state;

        // Save the current cell in the action table
        action = parser->parse_table->action_table[state][parse_table_get_terminal_index(token->token_type)];

        // If Action[state, token] == Shift
        if (action.action_type == Action_Shift)
        {
            // Create a new tree node from the current token
            tree_node = parse_tree_init_node(Terminal, token->token_type, token, NULL, 0);
            // Create a new stack entry for terminal 
            stack_entry = parse_stack_init_entry(tree_node, action.state_or_rule);
            // Push created stack entry onto the stack
            parse_stack_push(&(parser->parse_stack), stack_entry);
            // Get next token from the source code
            token = lexer_get_next_token(parser->lexer);
        }
        // If Action[state, token] == Reduce
        else if (action.action_type == Action_Reduce)
        {
            // Get the current production rule
            production_rule = parser->production_rules[action.state_or_rule];
            // Create an array of the size of number of symbols on the RHS of the production rule that we reduce by
            children = (Parse_Tree_Node**) calloc(production_rule.rule_length, sizeof(Parse_Tree_Node*));
            // Check allocation error
            if (children == NULL)
            {
                // Destroy parser
                parser_destroy(&parser);
                error_handler_report_memory_error();
            }
            // Pop Length(Production rule) entries from the stack, and put the trees of each entry as a child in the array.
            // From left of array to the right because the production rule is "reversed" in the stack
            for (i = production_rule.rule_length - 1; i >= 0; i--)
            {
                // Pop entry from the stack
                stack_entry = parse_stack_pop(&(parser->parse_stack));
                // Place its tree in the children array
                children[i] = stack_entry->tree;
                // Free poped entry
                free(stack_entry);
            }
            // Create a new tree node from the non-terminal on the LHS of the production rule we reduce by
            tree_node = parse_tree_init_node(Non_Terminal, production_rule.non_terminal_type, NULL, children, production_rule.rule_length);
            // Create a new stack entry using the created tree node and Goto table
            stack_entry = parse_stack_init_entry(tree_node, parser->parse_table->goto_table[parser->parse_stack->goto_state][production_rule.non_terminal_type]);
            // Push created entry onto the stack
            parse_stack_push(&(parser->parse_stack), stack_entry);
        }
        // If Action[state, token] == Accept
        else if (action.action_type == Action_Accept)
        {
            // When reached an Accept, the stack only has one entry other than the start entry, which contains the parse tree
            tree_node = parser->parse_stack->tree;
            // Disconnect tree node from entry so it won't be destroyed
            parser->parse_stack->tree = NULL;
            parser_destroy(&parser);
            // Returns the parse tree
            return tree_node;
        }
        // If Action[state, token] == Error
        else
        {
            // If reached an Error, save error line, destroy the parser, output error message and exit
            i = parser->lexer->line;
            parser_destroy(&parser);
            error_handler_report(i, Error_Parser, "Unexpected token `%s`", token->value);
        }
    }
}
