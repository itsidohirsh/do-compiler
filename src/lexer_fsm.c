#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/lexer_fsm.h"

typedef enum State_Name
{
    IDENTIFIER,
    P,
    PR,
    PRO,
    PROG,
    ASSIGNMENT,
    EQUAL,
} State_Name;

void lexer_fsm_init(Lexer_FSM* fsm)
{
    fsm->num_states = 0;

    int i;

    // Identifier
    lexer_fsm_add_state(fsm, IDENTIFIER, ' ', Token_Identifier, true);
    for (i = 0; i < NUM_CHARACTERS; i++)
        if (isalnum(i + LOWER_BOUND))
            lexer_fsm_add_edge(fsm, IDENTIFIER, i, IDENTIFIER);

    // "prog"
    lexer_fsm_add_state(fsm, P, 'p', Token_Error, false);
    lexer_fsm_add_state(fsm, PR, 'r', Token_Error, false);
    lexer_fsm_add_state(fsm, PRO, 'o', Token_Error, false);
    lexer_fsm_add_state(fsm, PROG, 'g', Token_Prog, true);
    lexer_fsm_add_edge(fsm, P, lexer_fsm_get_char_index('r'), PR);
    lexer_fsm_add_edge(fsm, PR, lexer_fsm_get_char_index('o'), PRO);
    lexer_fsm_add_edge(fsm, PRO, lexer_fsm_get_char_index('g'), PROG);

    // = & ==
    lexer_fsm_add_state(fsm, ASSIGNMENT, '=', Token_Assignment, true);
    lexer_fsm_add_state(fsm, EQUAL, '=', Token_Equal, true);
    lexer_fsm_add_edge(fsm, ASSIGNMENT, lexer_fsm_get_char_index('='), EQUAL);
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