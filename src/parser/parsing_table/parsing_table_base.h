#pragma once

#include "../../token/token.h"

// Number of terminals in the parasing table
// The terminals are the tokens. So the number of terminals is the same as the number of tokens
#define NUM_OF_TERMINALS 32
// Number of non-terminals in the parasing table
#define NUM_OF_NON_TERMINALS 14
// Number of states in the parser's table
#define PARSING_TABLE_NUM_OF_STATES 62

// Enum of all the possible actions a cell in the parsing table can do
typedef enum Action_Kind
{
    Action_Error,
    Action_Shift,
    Action_Reduce,
    Action_Accept
} Action_Kind;

// Enum of all terminals, Exactly the same as the Token_Kind enum
// Only for ease of use and readability
typedef Token_Kind Terminal_Kind;

// Enum of all the non-terminals in the grammar of the language
typedef enum Non_Terminal_Kind
{
    Non_Terminal_PROG,
    Non_Terminal_BLOCK,
    Non_Terminal_STMT,
    Non_Terminal_DECL,
    Non_Terminal_ASSIGN,
    Non_Terminal_IF_ELSE,
    Non_Terminal_WHILE,
    Non_Terminal_L_LOG_E,
    Non_Terminal_ELSE,
    Non_Terminal_H_LOG_E,
    Non_Terminal_BOOL_E,
    Non_Terminal_E,
    Non_Terminal_T,
    Non_Terminal_F,
} Non_Terminal_Kind;
