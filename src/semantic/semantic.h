#pragma once

#include <stdbool.h>


/* ---------- Structs ---------- */

// Enum of all the possible types of an identifier in the language.
// This enum is only for readability and ease of use.
typedef enum Data_Type
{
    Data_Type_NULL = 0,             // For tree nodes that are not part of an expression
    Data_Type_Int = Token_Int,      // int
    Data_Type_Char = Token_Char,    // char
} Data_Type;


/* ---------- Functions ---------- */

// Checks if it's possible to assing expression's type to the identifier's type
bool semantic_check_assign_compatibility(Data_Type id_type, Data_Type expr_type);

// Checks if the data type of the left and right operands is compatible with the operator.
// If yes returns the data type of the combined expression.
// If not return Data_Type_NULL
Data_Type semantic_check_operation_compatibility(Data_Type left_op, Token* operator, Data_Type right_op);

// Called after encountering :
// After : we know we are know in a new block, which means a new scope.
// Adds a scope child to the current scope, and moves the current scope to that child.
void semantic_enter_block();

// Called after reduction by the production rule: BLOCK -> done (R2)
// When done is encountered, we know we've came out of a scope,
// so move back to the parent scope in the scope tree.
void semantic_exit_block();

// Checks after reduction by the production rule: DECL -> data_type id ; (R7)
// if the identifier is declared in the current scope or not.
// If not inserts a new entry into the current scope's symbol with the known data.
// If yes, reports a semantic error of identifier already exists.
void semantic_decl();

// Checks after reduction by the production rule: ASSIGN -> set id = L_LOG_E ; (R8)
// if id exists in the symbol table, and if the type of that id equals the type of L_LOG_E.
// If either of the above is false, reports a semantic error for the matching error.
void semantic_assign();

// After reduction by a production rule of the form: Non_Terminal_1 -> Non_terminal_2
// sets Non_Terminal_1.type = Non_Terminal_2.type
void semantic_set_type();

// After reduction by any prodution rule with the form: Operand Operator Operand
// the 3 children of the tree at the top of the parse stack are the Operand Operator Operand accordingly.
// Checking if the type of the Operand and the Operand matches according to the limitation of the language.
// If the types match, sets the LHS_Non_Terminal.type = the matching type.
// If don't match, reports a type mismatch error.
void semantic_type_check();

// Checks after reduction by the production rule: F -> id (R23)
// Need to validate that the identifier exists, and if so need to set F.type = id.type
void semantic_F_to_id();

// After reduction by the production rule: F -> literal (R24)
// Need to set F.type = literal.type
void semantic_F_to_literal();

// After reduction by the production rule: F -> ( L_LOG_E ) (R25)
// Need to set F.type = L_LOG_E.type
void semantic_F_to_L_LOG_E();

// After reduction by the production rule: F -> ! F (R26)
// And reduction by the production rule: F -> - F (R27)
// Need to set F.type = F.type
void semantic_F_to_unary_op_F();

// Converts a Data_Type to it's string representation
const char* semantic_data_type_to_str(Data_Type data_type);
