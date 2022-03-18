#include <string.h>

#include "../global.h"

#include "semantic.h"
#include "../general/general.h"
#include "../scope_tree/scope_tree.h"
#include "../error_handler/error_handler.h"
#include "../token/token.h"


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

    // If the entry is NOT NULL, that means this identifier is already decleared, so report semantic error.
    else
        error_handler_report(compiler.lexer->line, Error_Semantic, "Identifier `%s` already exists", identifier);
}
