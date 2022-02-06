#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/lexer_fsm.h"

void lexer_fsm_init(Lexer_FSM* fsm)
{
    fsm->num_states = 0;

    /*** Identifier ***/
    lexer_fsm_add_state(fsm, IDENTIFIER, 'I', Token_Identifier, true);
    lexer_fsm_alnum_to_identifier(fsm, IDENTIFIER, 0);

    /*** Number ***/
    lexer_fsm_add_state(fsm, NUMBER, '#', Token_Number, true);
    int i;
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isdigit(i + LOWER_BOUND))
            lexer_fsm_add_edge(fsm, NUMBER, i, NUMBER);

    /*** "prog" ***/
    // states
    lexer_fsm_add_state(fsm, P, 'p', Token_Error, false);
    lexer_fsm_add_state(fsm, PR, 'r', Token_Error, false);
    lexer_fsm_add_state(fsm, PRO, 'o', Token_Error, false);
    lexer_fsm_add_state(fsm, PROG, 'g', Token_Prog, true);

    // edges
    lexer_fsm_add_edge(fsm, P, lexer_fsm_get_char_index('r'), PR);
    lexer_fsm_alnum_to_identifier(fsm, P, 'r');

    lexer_fsm_add_edge(fsm, PR, lexer_fsm_get_char_index('o'), PRO);
    lexer_fsm_alnum_to_identifier(fsm, PR, 'o');

    lexer_fsm_add_edge(fsm, PRO, lexer_fsm_get_char_index('g'), PROG);
    lexer_fsm_alnum_to_identifier(fsm, PRO, 'g');

    lexer_fsm_alnum_to_identifier(fsm, PROG, 0);

    /*** "if" ***/
    // states
    lexer_fsm_add_state(fsm, I, 'i', Token_Error, false);
    lexer_fsm_add_state(fsm, IF, 'f', Token_If, true);

    // edges
    lexer_fsm_add_edge(fsm, I, lexer_fsm_get_char_index('f'), IF);
    lexer_fsm_alnum_to_identifier(fsm, I, 'f');

    lexer_fsm_alnum_to_identifier(fsm, IF, 0);

    /*** "else" ***/
    // states
    lexer_fsm_add_state(fsm, E, 'e', Token_Error, false);
    lexer_fsm_add_state(fsm, EL, 'l', Token_Error, false);
    lexer_fsm_add_state(fsm, ELS, 's', Token_Error, false);
    lexer_fsm_add_state(fsm, ELSE, 'e', Token_Else, true);

    // edges
    lexer_fsm_add_edge(fsm, E, lexer_fsm_get_char_index('l'), EL);
    lexer_fsm_alnum_to_identifier(fsm, E, 'l');

    lexer_fsm_add_edge(fsm, EL, lexer_fsm_get_char_index('s'), ELS);
    lexer_fsm_alnum_to_identifier(fsm, EL, 's');

    lexer_fsm_add_edge(fsm, ELS, lexer_fsm_get_char_index('e'), ELSE);
    lexer_fsm_alnum_to_identifier(fsm, ELS, 'e');

    lexer_fsm_alnum_to_identifier(fsm, ELSE, 0);
}

void lexer_fsm_alnum_to_identifier(Lexer_FSM* fsm, State_Name state, char except)
{
    int i;
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isalnum(i + LOWER_BOUND) && (i + LOWER_BOUND) != except)
            lexer_fsm_add_edge(fsm, state, i, IDENTIFIER);
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
    case 'p':
        return P;

    case 'i':
        return I;

    case 'e':
        return E;

    default:
        if (isalpha(value))
            return IDENTIFIER;
        if (isdigit(value))
            return NUMBER;
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

void lexer_fsm_print_dot(Lexer_FSM* fsm)
{
    printf("\ndigraph {\n");
    int i, j;
    for (i = 0; i < NUM_STATES; i++)
        for (j = 0; j < NUM_CHARACTERS; j++)
        {
            if (fsm->edges[i][j].weight != 0)
            {
                if (fsm->edges[i][j].weight != IDENTIFIER)
                    printf("    %c -> %c\n", fsm->states[i].value, fsm->states[fsm->edges[i][j].weight].value);
            }
        }
    printf("}\n");
}