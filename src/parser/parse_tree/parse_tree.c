#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "parse_tree.h"
#include "../../general/general.h"


Parse_Tree_Node* parse_tree_init_node(Symbol_Type symbol_type, int symbol, Token* token, Parse_Tree_Node** children, int num_of_children)
{
    // Create a new Parse_Tree_Node
    Parse_Tree_Node* node = (Parse_Tree_Node*) calloc(1, sizeof(Parse_Tree_Node));
    if (node == NULL)
    {
        // Free all of the children trees in the children array
        for (int i = 0; i < num_of_children; i++)
            parse_tree_destroy(children[i]);

        // Free the children pointers array
        free(children);

        // Free the token
        token_destroy(token);

        exit_memory_error(__FILE__, __LINE__);
    }

    // Update node's properties
    node->symbol_type = symbol_type;
    node->symbol = symbol;
    node->token = token;
    node->children = children;
    node->num_of_children = num_of_children;

    return node;
}

void parse_tree_destroy(Parse_Tree_Node* root)
{
    // If reached a leaf stop recursion
    if (root == NULL)
        return;

    // Destroy children
    for (int i = 0; i < root->num_of_children; i++)
        parse_tree_destroy(root->children[i]);

    // Free the node's token
    token_destroy(root->token);

    // Destroy current node
    free(root->children);
    free(root);
}

// ----- Print tree helper functions -----

// Converts a non-terminal type to a string representation
const char* parser_tree_non_terminal_to_str(Non_Terminal_Type non_terminal_type);

// The recursive function that actually prints the parse tree
void parse_tree_print_tree_rec(Parse_Tree_Node* root, char* indent, bool is_last);

// Prints a single node in the parse tree
void parse_tree_print_node(Parse_Tree_Node* node);

const char* parser_tree_non_terminal_to_str(Non_Terminal_Type non_terminal_type)
{
    switch (non_terminal_type)
    {
        case Non_Terminal_PROG: return "PROG";
        case Non_Terminal_BLOCK: return "BLOCK";
        case Non_Terminal_STMT: return "STMT";
        case Non_Terminal_DECL: return "DECL";
        case Non_Terminal_ASSIGN: return "ASSIGN";
        case Non_Terminal_IF_ELSE: return "IF_ELSE";
        case Non_Terminal_WHILE: return "WHILE";
        case Non_Terminal_L_LOG_E: return "L_LOG_E";
        case Non_Terminal_ELSE: return "ELSE";
        case Non_Terminal_H_LOG_E: return "H_LOG_E";
        case Non_Terminal_BOOL_E: return "BOOL_E";
        case Non_Terminal_E: return "E";
        case Non_Terminal_T: return "T";
        case Non_Terminal_F: return "F";

        default: return "Don't know that non-terminal type... ;|";
    }
}

void parse_tree_print(Parse_Tree_Node* root)
{
    char indent[256] = { 0 };
    parse_tree_print_tree_rec(root, indent, true);
}

void parse_tree_print_node(Parse_Tree_Node* node)
{
    if (node->symbol_type == Terminal)
        printf("%s", token_to_str(node->token));
    else
        printf("%s", parser_tree_non_terminal_to_str(node->symbol));
}

void parse_tree_print_tree_rec(Parse_Tree_Node* root, char* indent, bool is_last)
{
    if (root == NULL)
        return;

    char marker[256] = " |---";
    char cur_indent[256] = " |   ";

    if (is_last)
    {
        sprintf(marker, " `---");
        sprintf(cur_indent, "     ");
    }

    printf("%s", indent);
    printf("%s", marker);

    printf(" ");
    parse_tree_print_node(root);
    printf("\n");

    strcat(indent, cur_indent);

    // Save current indentation before recursing
    char tmp[256];
    strcpy(tmp, indent);

    for (int i = 0; i < root->num_of_children; i++)
    {
        parse_tree_print_tree_rec(root->children[i], indent, i == root->num_of_children - 1);
        strcpy(indent, tmp);
    }
}
