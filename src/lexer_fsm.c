#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/lexer_fsm.h"

void lexer_fsm_init(Lexer_FSM* fsm)
{
    int i;

    fsm->num_states = 0;

    /*** Space ***/
    lexer_fsm_add_state(fsm, FS_Whitespace, ' ', Token_Whitespace, false);
    lexer_fsm_add_edge(fsm, FS_Whitespace, lexer_fsm_get_char_index(' '), FS_Whitespace);
    /*************/

    /*** Identifier ***/
    lexer_fsm_add_state(fsm, FS_Identifier, 'I', Token_Identifier, true);
    lexer_fsm_alnum_to_identifier(fsm, FS_Identifier, 0);
    /******************/

    /*** Number ***/
    lexer_fsm_add_state(fsm, FS_Number, '#', Token_Number, true);
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isdigit(i + LOWER_BOUND))
            lexer_fsm_add_edge(fsm, FS_Number, i, FS_Number);
    /**************/

    /*** Character ***/
    // states
    lexer_fsm_add_state(fsm, FS__, '\'', Token_Error, false);
    lexer_fsm_add_state(fsm, FS__C, 'C', Token_Error, false);
    lexer_fsm_add_state(fsm, FS__C_, '\'', Token_Character, true);

    // edges
    for (i = 0; i < NUM_CHARACTERS; i++)
        lexer_fsm_add_edge(fsm, FS__, i, FS__C);

    lexer_fsm_add_edge(fsm, FS__C, lexer_fsm_get_char_index('\''), FS__C_);
    /*************/

    /*** "int" ***/
    // states
    lexer_fsm_add_state(fsm, FS_I, 'i', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_iN, 'n', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_inT, 't', Token_Int, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_I, lexer_fsm_get_char_index('n'), FS_iN);
    lexer_fsm_alnum_to_identifier(fsm, FS_I, 'n');

    lexer_fsm_add_edge(fsm, FS_iN, lexer_fsm_get_char_index('t'), FS_inT);
    lexer_fsm_alnum_to_identifier(fsm, FS_iN, 't');

    lexer_fsm_alnum_to_identifier(fsm, FS_inT, 0);
    /*************/

    /*** "char" ***/
    // states
    lexer_fsm_add_state(fsm, FS_C, 'c', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_cH, 'h', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_chA, 'a', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_chaR, 'r', Token_Char, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_C, lexer_fsm_get_char_index('h'), FS_cH);
    lexer_fsm_alnum_to_identifier(fsm, FS_C, 'h');

    lexer_fsm_add_edge(fsm, FS_cH, lexer_fsm_get_char_index('a'), FS_chA);
    lexer_fsm_alnum_to_identifier(fsm, FS_cH, 'a');

    lexer_fsm_add_edge(fsm, FS_chA, lexer_fsm_get_char_index('r'), FS_chaR);
    lexer_fsm_alnum_to_identifier(fsm, FS_chA, 'r');

    lexer_fsm_alnum_to_identifier(fsm, FS_chaR, 0);
    /**************/

    /*** "prog" ***/
    // states
    lexer_fsm_add_state(fsm, FS_P, 'p', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_pR, 'r', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_prO, 'o', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_proG, 'g', Token_Prog, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_P, lexer_fsm_get_char_index('r'), FS_pR);
    lexer_fsm_alnum_to_identifier(fsm, FS_P, 'r');

    lexer_fsm_add_edge(fsm, FS_pR, lexer_fsm_get_char_index('o'), FS_prO);
    lexer_fsm_alnum_to_identifier(fsm, FS_pR, 'o');

    lexer_fsm_add_edge(fsm, FS_prO, lexer_fsm_get_char_index('g'), FS_proG);
    lexer_fsm_alnum_to_identifier(fsm, FS_prO, 'g');

    lexer_fsm_alnum_to_identifier(fsm, FS_proG, 0);
    /**************/

    /*** "if" ***/
    // states
    lexer_fsm_add_state(fsm, FS_iF, 'f', Token_If, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_I, lexer_fsm_get_char_index('f'), FS_iF);

    lexer_fsm_alnum_to_identifier(fsm, FS_iF, 0);
    /************/

    /*** "else" ***/
    // states
    lexer_fsm_add_state(fsm, FS_E, 'e', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_eL, 'l', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_elS, 's', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_elsE, 'e', Token_Else, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_E, lexer_fsm_get_char_index('l'), FS_eL);
    lexer_fsm_alnum_to_identifier(fsm, FS_E, 'l');

    lexer_fsm_add_edge(fsm, FS_eL, lexer_fsm_get_char_index('s'), FS_elS);
    lexer_fsm_alnum_to_identifier(fsm, FS_eL, 's');

    lexer_fsm_add_edge(fsm, FS_elS, lexer_fsm_get_char_index('e'), FS_elsE);
    lexer_fsm_alnum_to_identifier(fsm, FS_elS, 'e');

    lexer_fsm_alnum_to_identifier(fsm, FS_elsE, 0);
    /**************/

    /*** "while" ***/
    // states
    lexer_fsm_add_state(fsm, FS_W, 'w', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_wH, 'h', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_whI, 'i', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_whiL, 'l', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_whilE, 'e', Token_While, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_W, lexer_fsm_get_char_index('h'), FS_wH);
    lexer_fsm_alnum_to_identifier(fsm, FS_W, 'h');

    lexer_fsm_add_edge(fsm, FS_wH, lexer_fsm_get_char_index('i'), FS_whI);
    lexer_fsm_alnum_to_identifier(fsm, FS_wH, 'i');

    lexer_fsm_add_edge(fsm, FS_whI, lexer_fsm_get_char_index('l'), FS_whiL);
    lexer_fsm_alnum_to_identifier(fsm, FS_whI, 'l');

    lexer_fsm_add_edge(fsm, FS_whiL, lexer_fsm_get_char_index('e'), FS_whilE);
    lexer_fsm_alnum_to_identifier(fsm, FS_whiL, 'e');

    lexer_fsm_alnum_to_identifier(fsm, FS_whilE, 0);
    /***************/

    /*** "set" ***/
    // states
    lexer_fsm_add_state(fsm, FS_S, 's', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_sE, 'e', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_seT, 't', Token_Set, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_S, lexer_fsm_get_char_index('e'), FS_sE);
    lexer_fsm_alnum_to_identifier(fsm, FS_S, 'e');

    lexer_fsm_add_edge(fsm, FS_sE, lexer_fsm_get_char_index('t'), FS_seT);
    lexer_fsm_alnum_to_identifier(fsm, FS_sE, 't');

    lexer_fsm_alnum_to_identifier(fsm, FS_seT, 0);
    /*************/

    /*** "done" ***/
    // states
    lexer_fsm_add_state(fsm, FS_D, 'd', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_dO, 'o', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_doN, 'n', Token_Identifier, false);
    lexer_fsm_add_state(fsm, FS_donE, 'e', Token_Done, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_D, lexer_fsm_get_char_index('o'), FS_dO);
    lexer_fsm_alnum_to_identifier(fsm, FS_D, 'o');

    lexer_fsm_add_edge(fsm, FS_dO, lexer_fsm_get_char_index('n'), FS_doN);
    lexer_fsm_alnum_to_identifier(fsm, FS_dO, 'n');

    lexer_fsm_add_edge(fsm, FS_doN, lexer_fsm_get_char_index('e'), FS_donE);
    lexer_fsm_alnum_to_identifier(fsm, FS_doN, 'e');

    lexer_fsm_alnum_to_identifier(fsm, FS_donE, 0);
    /**************/

    /*** = & == ***/
    // states
    lexer_fsm_add_state(fsm, FS_Assignment, '=', Token_Assignment, true);
    lexer_fsm_add_state(fsm, FS_Equal, '=', Token_Equal, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_Assignment, lexer_fsm_get_char_index('='), FS_Equal);
    /**************/

    /*** ! & != ***/
    // states
    lexer_fsm_add_state(fsm, FS_Not, '!', Token_Not, true);
    lexer_fsm_add_state(fsm, FS_Not_Equal, '=', Token_Not_Equal, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_Not, lexer_fsm_get_char_index('='), FS_Not_Equal);
    /**************/

    /*** > & >= ***/
    // states
    lexer_fsm_add_state(fsm, FS_Bigger, '>', Token_Bigger, true);
    lexer_fsm_add_state(fsm, FS_Bigger_Equal, '=', Token_Bigger_Equal, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_Bigger, lexer_fsm_get_char_index('='), FS_Bigger_Equal);
    /**************/

    /*** < & <= ***/
    // states
    lexer_fsm_add_state(fsm, FS_Smaller, '<', Token_Smaller, true);
    lexer_fsm_add_state(fsm, FS_Smaller_Equal, '=', Token_Smaller_Equal, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_Smaller, lexer_fsm_get_char_index('='), FS_Smaller_Equal);
    /**************/

    /*** || ***/
    // states
    lexer_fsm_add_state(fsm, FS_or, '|', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_OR, '|', Token_Or, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_or, lexer_fsm_get_char_index('|'), FS_OR);
    /**************/

    /*** && ***/
    // states
    lexer_fsm_add_state(fsm, FS_and, '&', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_AND, '&', Token_And, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_and, lexer_fsm_get_char_index('&'), FS_AND);
    /**************/

    /*** + - * / % ( ) : ; ***/
    lexer_fsm_add_state(fsm, FS_Plus, '+', Token_Plus, true);
    lexer_fsm_add_state(fsm, FS_Minus, '-', Token_Minus, true);
    lexer_fsm_add_state(fsm, FS_Multiply, '*', Token_Multiply, true);
    lexer_fsm_add_state(fsm, FS_Divide, '/', Token_Divide, true);
    lexer_fsm_add_state(fsm, FS_Modulu, '%', Token_Modulu, true);
    lexer_fsm_add_state(fsm, FS_Open_Paren, '(', Token_Open_Paren, true);
    lexer_fsm_add_state(fsm, FS_Close_Paren, ')', Token_Close_Paren, true);
    lexer_fsm_add_state(fsm, FS_Colon, ':', Token_Colon, true);
    lexer_fsm_add_state(fsm, FS_Semi_Colon, ';', Token_Semi_Colon, true);
}

void lexer_fsm_alnum_to_identifier(Lexer_FSM* fsm, FS_Name state, char except)
{
    int i;
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isalnum(i + LOWER_BOUND) && (i + LOWER_BOUND) != except)
            lexer_fsm_add_edge(fsm, state, i, FS_Identifier);
}

void lexer_fsm_add_state(Lexer_FSM* fsm, int index, char value, Token_Type token_type, bool is_final_state)
{
    fsm->states[index].value = value;
    fsm->states[index].token_type = token_type;
    fsm->states[index].is_final_state = is_final_state;
    fsm->num_states++;
}

void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char weight)
{
    fsm->edges[from][to].weight = weight;
}

int lexer_fsm_get_char_index(char value)
{
    // If the character is out of range return the first index in the array, 0
    // Else reutrn it's proper index
    return (value - LOWER_BOUND >= 0) ? (value - LOWER_BOUND) : 0;
}

int lexer_fsm_get_state_index(char value)
{
    // If i - start of "int" and "if"
    if (value == 'i')
        return FS_I;

    // If c - start of "char"
    if (value == 'c')
        return FS_C;

    // If p - start of "prog"
    if (value == 'p')
        return FS_P;

    // If e - start of "else"
    if (value == 'e')
        return FS_E;

    // If w - start of "while"
    if (value == 'w')
        return FS_W;

    // If s - start of "set"
    if (value == 's')
        return FS_S;

    // If d - start of "done"
    if (value == 'd')
        return FS_D;

    // If ' - start of Character literal
    if (value == '\'')
        return FS__;

    // If = - start of = | ==
    if (value == '=')
        return FS_Assignment;

    // If ! - start of ! | !=
    if (value == '!')
        return FS_Not;

    // If > - start of > | >=
    if (value == '>')
        return FS_Bigger;

    // If < - start of < | <=
    if (value == '<')
        return FS_Smaller;

    // If | - start of ||
    if (value == '|')
        return FS_or;

    // If & - start of &&
    if (value == '&')
        return FS_and;

    // If +
    if (value == '+')
        return FS_Plus;

    // If -
    if (value == '-')
        return FS_Minus;

    // If *
    if (value == '*')
        return FS_Multiply;

    // If /
    if (value == '/')
        return FS_Divide;

    // If %
    if (value == '%')
        return FS_Modulu;

    // If (
    if (value == '(')
        return FS_Open_Paren;

    // If )
    if (value == ')')
        return FS_Close_Paren;

    // If :
    if (value == ':')
        return FS_Colon;

    // If ;
    if (value == ';')
        return FS_Semi_Colon;

    // If whitespace
    if (isspace(value))
        return FS_Whitespace;

    // If letter - start of Identifier
    if (isalpha(value))
        return FS_Identifier;

    // If digit - start of number
    if (isdigit(value))
        return FS_Number;

    // If doesn't match anything else, return 0
    return 0;
}

void lexer_fsm_print(Lexer_FSM* fsm)
{
    int i, j;

    // Vertices
    printf("num_states = %d\n", fsm->num_states);
    for (i = 0; i < NUM_STATES; i++)
    {
        printf("V[%2d]: (`%c`, %d)\n", i, fsm->states[i].value, fsm->states[i].token_type);
    }
    printf("\n");

    // Edges
    printf("        ");
    for (i = 0; i < NUM_CHARACTERS; i++)
    {
        printf("%-2c", i + LOWER_BOUND);
    }
    printf("\n");

    printf("        ");
    for (i = 0; i < NUM_CHARACTERS; i++)
    {
        printf("%-2c", '_');
    }
    printf("\n");

    for (i = 0; i < NUM_STATES; i++)
    {
        if (fsm->states[i].value != 0)
            printf("%2d. %c | ", i, fsm->states[i].value);
        else
            printf("%2d.   | ", i);

        for (j = 0; j < NUM_CHARACTERS; j++)
        {
            if (fsm->edges[i][j].weight != 0)
                printf("%-2d", fsm->edges[i][j].weight);
            else
                printf("%-2c", '.');
        }
        printf("\n");
    }
    printf("\n");
}
