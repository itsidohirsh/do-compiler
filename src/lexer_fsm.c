#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/lexer_fsm.h"

void lexer_fsm_init(Lexer_FSM* fsm)
{
    fsm->num_states = 0;

    /*** Space ***/
    lexer_fsm_add_state(fsm, FS_Whitespace, ' ', Token_Whitespace, false);
    lexer_fsm_add_edge(fsm, FS_Whitespace, lexer_fsm_get_char_index(' '), FS_Whitespace);

    /*** Identifier ***/
    lexer_fsm_add_state(fsm, FS_Identifier, ' ', Token_Identifier, true);
    lexer_fsm_alnum_to_identifier(fsm, FS_Identifier, 0);

    /*** Number ***/
    lexer_fsm_add_state(fsm, FS_Number, ' ', Token_Number, true);
    int i;
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isdigit(i + LOWER_BOUND))
            lexer_fsm_add_edge(fsm, FS_Number, i, FS_Number);

    /*** "prog" ***/
    // states
    lexer_fsm_add_state(fsm, FS_P, 'p', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_pR, 'r', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_prO, 'o', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_proG, 'g', Token_Prog, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_P, lexer_fsm_get_char_index('r'), FS_pR);
    lexer_fsm_alnum_to_identifier(fsm, FS_P, 'r');

    lexer_fsm_add_edge(fsm, FS_pR, lexer_fsm_get_char_index('o'), FS_prO);
    lexer_fsm_alnum_to_identifier(fsm, FS_pR, 'o');

    lexer_fsm_add_edge(fsm, FS_prO, lexer_fsm_get_char_index('g'), FS_proG);
    lexer_fsm_alnum_to_identifier(fsm, FS_prO, 'g');

    lexer_fsm_alnum_to_identifier(fsm, FS_proG, 0);

    /*** "if" ***/
    // states
    lexer_fsm_add_state(fsm, FS_I, 'i', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_iF, 'f', Token_If, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_I, lexer_fsm_get_char_index('f'), FS_iF);
    lexer_fsm_alnum_to_identifier(fsm, FS_I, 'f');

    lexer_fsm_alnum_to_identifier(fsm, FS_iF, 0);

    /*** "else" ***/
    // states
    lexer_fsm_add_state(fsm, FS_E, 'e', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_eL, 'l', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_elS, 's', Token_Error, false);
    lexer_fsm_add_state(fsm, FS_elsE, 'e', Token_Else, true);

    // edges
    lexer_fsm_add_edge(fsm, FS_E, lexer_fsm_get_char_index('l'), FS_eL);
    lexer_fsm_alnum_to_identifier(fsm, FS_E, 'l');

    lexer_fsm_add_edge(fsm, FS_eL, lexer_fsm_get_char_index('s'), FS_elS);
    lexer_fsm_alnum_to_identifier(fsm, FS_eL, 's');

    lexer_fsm_add_edge(fsm, FS_elS, lexer_fsm_get_char_index('e'), FS_elsE);
    lexer_fsm_alnum_to_identifier(fsm, FS_elS, 'e');

    lexer_fsm_alnum_to_identifier(fsm, FS_elsE, 0);
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
    return value - LOWER_BOUND;
}

int lexer_fsm_get_state_index(char value)
{
    switch (value)
    {
        // If whitespace
    case ' ':
    case '\n':
    case '\t':
        return FS_Whitespace;

        // If p - start of "prog"
    case 'p':
        return FS_P;

        // If i - start of "if"
    case 'i':
        return FS_I;

        // If e - start of "else"
    case 'e':
        return FS_E;

    default:
        // If letter - start of Identifier
        if (isalpha(value))
            return FS_Identifier;
        // If digit - start of number
        if (isdigit(value))
            return FS_Number;
    }
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
