#include <stdlib.h>

#include "parse_tree.h"
#include "../error_handler/error_handler.h"

Parse_Tree_Node* parse_tree_init_node(Parse_Tree_Node_Type node_type, int symbol, Token* token, Parse_Tree_Node** children, int num_of_children)
{
    // Create a new Parse_Tree_Node
    Parse_Tree_Node* node = (Parse_Tree_Node*) calloc(1, sizeof(Parse_Tree_Node));
    // Check for allocation error
    if (node == NULL)
    {
        // Free all of the children in the children array
        int i;
        for (i = 0; i < num_of_children; i++)
            parse_tree_destroy_tree(children[i]);

        // Free the token
        token_destroy(token);

        // Free the children pointers array
        free(children);

        // Report allocation error and exit
        error_handler_report_alloc();
    }

    // Update node's properties
    node->node_type = node_type;
    node->symbol = symbol;
    node->token = token;
    node->children = children;
    node->num_of_children = num_of_children;

    return node;
}

void parse_tree_destroy_tree(Parse_Tree_Node* root)
{
    // If reached a leaf stop recursion
    if (root == NULL)
        return;

    // Destroy children
    int i;
    for (i = 0; i < root->num_of_children; i++)
        parse_tree_destroy_tree(root->children[i]);

    // Free the node's token
    token_destroy(root->token);

    // Destroy current node
    free(root->children);
    free(root);
}

void parse_tree_print_tree(Parse_Tree_Node* root)
{
    // TODO: Implement print_tree() function
}
