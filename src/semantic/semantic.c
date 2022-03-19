#include <string.h>

#include "../global.h"

#include "semantic.h"
#include "../general/general.h"
#include "../scope_tree/scope_tree.h"
#include "../error_handler/error_handler.h"
#include "../token/token.h"


bool semantic_check_assign_compatibility(Data_Type id_type, Data_Type expr_type)
{
    // For now this will always return true, because I only have int and char in my language.
    // If I'll add more types this will have more logic.
    return true;
}

Data_Type semantic_check_binary_op_compatibility(Data_Type left_op, Token* operator, Data_Type right_op)
{
    // For now this will always return Data_Type_Int, because I only have int and char in my language,
    // And all the operations between int and char should return int type.
    // If I'll add more types this will have more logic.
    return Data_Type_Int;
}

Data_Type semantic_check_unary_op_compatibility(Data_Type operand, Token* operator)
{
    // For now this will always return Data_Type_Int, because I only have int and char in my language,
    // And all the operations on int or char should return int type.
    // If I'll add more types this will have more logic.
    return Data_Type_Int;
}

void semantic_enter_block()
{
    // Adds a new child scope for the current scope
    scope_tree_add_scope();

    // Advance to that new sub scope
    scope_tree_goto_child();
}

void semantic_exit_block()
{
    // Just go the the parent scope in the scope tree
    scope_tree_goto_parent();
}

void semantic_decl()
{
    // After reduction by the production rule: DECL -> data_type id ;

    // The identifier is the 2nd child in the tree that is currently on top of the stack (Because it's after reduce).
    // The identifier is the value of the token of the 2nd child.
    char* identifier = compiler.parser->parse_stack->tree->children[1]->token->value;

    // Fetch the identifier entry from current scope.
    Symbol_Table_Entry* entry = symbol_table_fetch(compiler.scope_tree->current_scope->symbol_table, identifier);

    // If the entry is NULL, insert identifier to the current scope.
    if (entry == NULL)
    {
        // I'm using strdup(identifier) because the symbol table and a token in the tree should not point to the same memory location.
        // In order to prevent segfaults when destroying the compiler.
        identifier = strdup(compiler.parser->parse_stack->tree->children[1]->token->value);
        if (identifier == NULL) exit_memory_error(__FILE__, __LINE__);

        // Insert new symbol table entry with the known data in to the current scope's symbol table.
        entry = symbol_table_entry_init(Entry_Type_Variable, identifier, compiler.parser->parse_stack->tree->children[0]->symbol);
        symbol_table_insert(compiler.scope_tree->current_scope->symbol_table, entry);
    }

    // If the entry is NOT NULL, that means this identifier is already declared, so report semantic error.
    else
        // compiler.lexer->line - 1 because after reduction the lexer's line was probably already advanced.
        // So for better error reporting I subtructed 1.
        error_handler_report(compiler.lexer->line - 1, Error_Semantic, "'%s' already declared", identifier);
}

void semantic_assign()
{
    // After reduction by the production rule: ASSIGN -> set id = L_LOG_E ;

    // Get the L_LOG_E from the tree at the top of the stack
    Parse_Tree_Node* L_LOG_E = compiler.parser->parse_stack->tree->children[3];

    // Get the identifier from the tree at the top of the stack
    char* identifier = compiler.parser->parse_stack->tree->children[1]->token->value;
    // Try to fetch the identifier from the scope tree
    Symbol_Table_Entry* entry = scope_tree_fetch(identifier);

    // If the entry is NULL that means the identifier was not found in the scope tree,
    // which means the programmer tries to assign value to an undeclared variable.
    if (entry == NULL)
        error_handler_report(compiler.lexer->line - 1, Error_Semantic, "'%s' undeclared", identifier);

    // Check for matching types
    else if (semantic_check_assign_compatibility(entry->data_type, L_LOG_E->data_type) == false)
        error_handler_report(compiler.lexer->line - 1, Error_Semantic, "Assignment of type '%s' to identifier '%s' of type '%s'", semantic_data_type_to_str(L_LOG_E->data_type), identifier, semantic_data_type_to_str(entry->data_type));
}

void semantic_set_type()
{
    compiler.parser->parse_stack->tree->data_type = compiler.parser->parse_stack->tree->children[0]->data_type;
}

void semantic_type_check()
{
    // Get the left operand from the tree at the top of the stack
    Parse_Tree_Node* left_op = compiler.parser->parse_stack->tree->children[0];

    // Get the operator from the tree at the top of the stack
    Parse_Tree_Node* operator = compiler.parser->parse_stack->tree->children[1];

    // Get the right operand from the tree at the top of the stack
    Parse_Tree_Node* right_op = compiler.parser->parse_stack->tree->children[2];

    // If the types match with that operator
    Data_Type result_type = semantic_check_binary_op_compatibility(left_op->data_type, operator->token, right_op->data_type);
    if (result_type != Data_Type_NULL)
        // Set the parent type to that type
        compiler.parser->parse_stack->tree->data_type = result_type;

    else
        error_handler_report(compiler.lexer->line, Error_Semantic, "Invalid operands to operator %s, have '%s' and '%s' ", operator->token->value, semantic_data_type_to_str(left_op->data_type), semantic_data_type_to_str(right_op->data_type));
}

void semantic_F_to_id()
{
    // Get the identifier from the tree at the top of the stack
    char* identifier = compiler.parser->parse_stack->tree->children[0]->token->value;
    // Try to fetch the identifier from the scope tree
    Symbol_Table_Entry* entry = scope_tree_fetch(identifier);

    // If Identifier exists
    if (entry != NULL)
        // F.type = id.type
        compiler.parser->parse_stack->tree->data_type = entry->data_type;

    else
        error_handler_report(compiler.lexer->line, Error_Semantic, "'%s' undeclared", identifier);
}

void semantic_F_to_literal()
{
    // Right now I only have int an char literals in my programming language.
    // My compiler interprets both of them as int literals.
    // So for now I just set every literal to int literal.
    // If I'll add more literal to my language then I'll add a translation function from literal to data type.
    compiler.parser->parse_stack->tree->data_type = Data_Type_Int;
}

void semantic_F_to_L_LOG_E()
{
    //      0    1    2
    // F -> ( L_LOG_E )
    compiler.parser->parse_stack->tree->data_type = compiler.parser->parse_stack->tree->children[1]->data_type;
}

void semantic_F_to_unary_op_F()
{
    // Get the operand from the tree at the top of the stack
    Parse_Tree_Node* operand = compiler.parser->parse_stack->tree->children[1];

    // Get the operator from the tree at the top of the stack
    Parse_Tree_Node* operator = compiler.parser->parse_stack->tree->children[0];

    compiler.parser->parse_stack->tree->data_type = semantic_check_unary_op_compatibility(operand->data_type, operator->token);
}

const char* semantic_data_type_to_str(Data_Type data_type)
{
    switch (data_type)
    {
        case Data_Type_Int: return "int";
        case Data_Type_Char: return "char";

        default: return "data_type";
    }
}
