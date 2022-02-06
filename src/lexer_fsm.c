#include <stdio.h>
#include <stdbool.h>

#include "../include/lexer_fsm.h"

void lexer_fsm_init(Lexer_FSM* fsm)
{
    fsm->num_states = 0;

    // "prog"
    lexer_fsm_add_state(fsm, 'p', Token_Identifier, false);
    lexer_fsm_add_state(fsm, 'r', Token_Identifier, false);
    lexer_fsm_add_state(fsm, 'o', Token_Identifier, false);
    lexer_fsm_add_state(fsm, 'g', Token_Prog, true);
    lexer_fsm_add_edge(fsm, lexer_fsm_get_index('p'), lexer_fsm_get_index('r'), 'r');
    lexer_fsm_add_edge(fsm, lexer_fsm_get_index('r'), lexer_fsm_get_index('o'), 'o');
    lexer_fsm_add_edge(fsm, lexer_fsm_get_index('o'), lexer_fsm_get_index('g'), 'g');
}

int lexer_fsm_get_index(char value)
{
    return value - LOWER_BOUND;
}

void lexer_fsm_add_state(Lexer_FSM* fsm, char value, Token_Type token_type, bool is_final_state)
{
    int index = lexer_fsm_get_index(value);
    fsm->states[index].value = value;
    fsm->states[index].token_type = token_type;
    fsm->states[index].is_final_state = is_final_state;
    fsm->num_states++;
}

void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char weight)
{
    fsm->edges[from][to].weight = weight;
}

void lexer_fsm_print(Lexer_FSM* fsm)
{
    int i, j;

    // Vertices
    printf("\nVertices\n");
    printf("num_states = %d\n", fsm->num_states);
    for (i = 0; i < NUM_STATES; i++)
    {
        printf("V[%2d]: (%c, %d)\n", i, fsm->states[i].value, fsm->states[i].token_type);
    }

    // Edges
    printf("\nEdges\n");
    printf("  ");
    for (i = 0; i < NUM_STATES; i++)
    {
        if (fsm->states[i].value != 0)
            printf("%-2c", fsm->states[i].value);
        else
            printf("  ");
    }
    printf("\n");

    for (i = 0; i < NUM_STATES; i++)
    {
        if (fsm->states[i].value != 0)
            printf("%c ", fsm->states[i].value);
        else
            printf("  ");

        for (j = 0; j < NUM_STATES; j++)
        {
            if (fsm->edges[i][j].weight != 0)
                printf("%-2c", fsm->edges[i][j].weight);
            else
                printf("%-2c", '.');
        }
        printf("\n");
    }
    printf("\n");
}