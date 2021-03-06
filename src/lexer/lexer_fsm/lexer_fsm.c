#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "../../global.h"

#include "lexer_fsm.h"
#include "../../general/general.h"
#include "../../error_handler/error_handler.h"


void lexer_fsm_create()
{
    // Create fsm
    compiler.lexer->fsm = (Lexer_FSM*) calloc(1, sizeof(Lexer_FSM));
    if (compiler.lexer->fsm == NULL) exit_memory_error(__FILE__, __LINE__);
}

void lexer_fsm_destroy()
{
    // check for NULL pointer
    if (compiler.lexer != NULL)
    {
        free(compiler.lexer->fsm);
        compiler.lexer->fsm = NULL;
    }
}

void lexer_fsm_set_alnum_identifier(int state, char except)
{
    int i;
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        if ((isalnum(i) || i == '_') && i != except)
            lexer_fsm_add_edge(state, i, lexer_fsm_get_starting_state_index('_'));
}

int lexer_fsm_get_char_index(char value)
{
    // If the character is out of range return the first index in the array, 0
    // Else reutrn it's proper index
    return (value >= 0) ? value : 0;
}

int lexer_fsm_get_starting_state_index(char value)
{
    return compiler.lexer->fsm->starting_state_indices[lexer_fsm_get_char_index(value)];
}

void lexer_fsm_add_starting_state_index(int char_index, int starting_state_index)
{
    compiler.lexer->fsm->starting_state_indices[char_index] = starting_state_index;
}

void lexer_fsm_add_state(int state_number, Token_Type token_type)
{
    compiler.lexer->fsm->states[state_number].token_type = token_type;
}

void lexer_fsm_add_edge(int state, int ch, int to_state)
{
    compiler.lexer->fsm->edges[state][ch].state_number = to_state;
}

void lexer_fsm_print()
{
    int i, j;

    // Edges
    printf("      ");
    for (i = 0; i < NUM_OF_CHARACTERS + 0; i++)
    {
        if (i >= ' ')
            printf("%c", i);
        else
            printf("%d", i % 10);
    }
    printf("\n");

    printf("      ");
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
    {
        printf("%c", '_');
    }
    printf("\n");

    for (i = 0; i < LEXER_FSM_NUM_OF_STATES; i++)
    {
        printf("%2d. | ", i);

        for (j = 0; j < NUM_OF_CHARACTERS; j++)
        {
            if (compiler.lexer->fsm->edges[i][j].state_number != 0)
                printf("%d", compiler.lexer->fsm->edges[i][j].state_number);
            else
                printf("%c", '.');
        }
        printf("\n");
    }
    printf("\n");
}

void lexer_fsm_init()
{
    // Variable to create current state index for each state in the FSM of the lexer.
    int s = 1;
    // Iterator
    int i;

    // - Whitespace -> 1 state
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index(' '), s);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\t'), s);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\r'), s);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\n'), s);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\v'), s);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\f'), s);
    // -- states
    lexer_fsm_add_state(s, Token_Whitespace);
    // -- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index(' '), s);
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('\t'), s);
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('\r'), s);
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('\n'), s);
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('\v'), s);
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('\f'), s);

    // - Comment -> 1 state
    s += 1; // To calculate s add to it the numer of states in the previous part
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('#'), s);
    // -- states
    lexer_fsm_add_state(s, Token_Comment);
    // -- edges
    // Starting from 0x20 because this is the space character and the first character we care about in a comment 
    for (i = 0x20; i < NUM_OF_CHARACTERS; i++)
        lexer_fsm_add_edge(s, lexer_fsm_get_char_index(i), s);


    // - Identifier -> 1 state
    s += 1;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('_'), s);
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        if (isalpha(i))
            lexer_fsm_add_starting_state_index(i, s);
    // -- states
    lexer_fsm_add_state(s, Token_Identifier);
    // -- edges
    lexer_fsm_set_alnum_identifier(s, 0);

    // - Number literal -> 3 state
    s += 1;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('0'), s);      // s     -> Starting at a state for first digit 0
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('1'), s + 1);  // s + 1 -> Starting at a state for first digit different from 0
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('2'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('3'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('4'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('5'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('6'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('7'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('8'), s + 1);
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('9'), s + 1);
    // -- states
    lexer_fsm_add_state(s, Token_Number); // State for first digit 0
    lexer_fsm_add_state(s + 1, Token_Number); // State for first digit other digits
    lexer_fsm_add_state(s + 2, Token_Error); // If there are continues 0s at the start of a number
    // -- edges
    // --- Add edge for 0-9 from the first digit 0 state to an error state because if a number starts with 0 it must not continue
    lexer_fsm_add_edge(s, '0', s + 2);
    lexer_fsm_add_edge(s, '1', s + 2);
    lexer_fsm_add_edge(s, '2', s + 2);
    lexer_fsm_add_edge(s, '3', s + 2);
    lexer_fsm_add_edge(s, '4', s + 2);
    lexer_fsm_add_edge(s, '5', s + 2);
    lexer_fsm_add_edge(s, '6', s + 2);
    lexer_fsm_add_edge(s, '7', s + 2);
    lexer_fsm_add_edge(s, '8', s + 2);
    lexer_fsm_add_edge(s, '9', s + 2);
    // --- Stay in first digit different from 0 state for every 0-9 digit
    lexer_fsm_add_edge(s + 1, '0', s + 1);
    lexer_fsm_add_edge(s + 1, '1', s + 1);
    lexer_fsm_add_edge(s + 1, '2', s + 1);
    lexer_fsm_add_edge(s + 1, '3', s + 1);
    lexer_fsm_add_edge(s + 1, '4', s + 1);
    lexer_fsm_add_edge(s + 1, '5', s + 1);
    lexer_fsm_add_edge(s + 1, '6', s + 1);
    lexer_fsm_add_edge(s + 1, '7', s + 1);
    lexer_fsm_add_edge(s + 1, '8', s + 1);
    lexer_fsm_add_edge(s + 1, '9', s + 1);

    // - Character literal -> 3 states
    s += 3;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('\''), s);
    // -- states
    lexer_fsm_add_state(s, Token_Error); // '
    lexer_fsm_add_state(s + 1, Token_Error); // 'C
    lexer_fsm_add_state(s + 2, Token_Character); // 'C'
    // -- edges
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        lexer_fsm_add_edge(s, i, s + 1);
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('\''), s + 2);

    // - Keywords
    // -- "int" & "if" -> 4 states
    s += 3;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('i'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // i
    lexer_fsm_add_state(s + 1, Token_If); // if
    lexer_fsm_add_state(s + 2, Token_Identifier); // in
    lexer_fsm_add_state(s + 3, Token_Int); // int
    // --- edges
    // ---- "int"
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('n'), s + 2);
    lexer_fsm_set_alnum_identifier(s, 'n');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('t'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 't');
    lexer_fsm_set_alnum_identifier(s + 3, 0);
    // ---- "if"
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('f'), s + 1);
    lexer_fsm_set_alnum_identifier(s + 1, 0);

    // -- "char" -> 4 states
    s += 4;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('c'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // c
    lexer_fsm_add_state(s + 1, Token_Identifier); // ch
    lexer_fsm_add_state(s + 2, Token_Identifier); // cha
    lexer_fsm_add_state(s + 3, Token_Char); // char
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('h'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'h');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('a'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 'a');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('r'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 'r');
    lexer_fsm_set_alnum_identifier(s + 3, 0);

    // -- "prog" -> 4 states
    s += 4;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('p'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // p
    lexer_fsm_add_state(s + 1, Token_Identifier); // pr
    lexer_fsm_add_state(s + 2, Token_Identifier); // pro
    lexer_fsm_add_state(s + 3, Token_Prog); // prog
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('r'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'r');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('o'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 'o');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('g'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 'g');
    lexer_fsm_set_alnum_identifier(s + 3, 0);

    // -- "else" -> 4 states
    s += 4;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('e'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // e
    lexer_fsm_add_state(s + 1, Token_Identifier); // el
    lexer_fsm_add_state(s + 2, Token_Identifier); // els
    lexer_fsm_add_state(s + 3, Token_Else); // else
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('l'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'l');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('s'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 's');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('e'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 'e');
    lexer_fsm_set_alnum_identifier(s + 3, 0);

    // -- "while" -> 5 states
    s += 4;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('w'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // w
    lexer_fsm_add_state(s + 1, Token_Identifier); // wh
    lexer_fsm_add_state(s + 2, Token_Identifier); // whi
    lexer_fsm_add_state(s + 3, Token_Identifier); // whil
    lexer_fsm_add_state(s + 4, Token_While); // while
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('h'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'h');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('i'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 'i');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('l'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 'l');
    lexer_fsm_add_edge(s + 3, lexer_fsm_get_char_index('e'), s + 4);
    lexer_fsm_set_alnum_identifier(s + 3, 'e');
    lexer_fsm_set_alnum_identifier(s + 4, 0);

    // -- "set" -> 3 states
    s += 5;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('s'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // s
    lexer_fsm_add_state(s + 1, Token_Identifier); // se
    lexer_fsm_add_state(s + 2, Token_Set); // set
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('e'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'e');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('t'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 't');
    lexer_fsm_set_alnum_identifier(s + 2, 0);

    // -- "done" -> 4 states
    s += 3;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('d'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Identifier); // d
    lexer_fsm_add_state(s + 1, Token_Identifier); // do
    lexer_fsm_add_state(s + 2, Token_Identifier); // don
    lexer_fsm_add_state(s + 3, Token_Done); // done
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('o'), s + 1);
    lexer_fsm_set_alnum_identifier(s, 'o');
    lexer_fsm_add_edge(s + 1, lexer_fsm_get_char_index('n'), s + 2);
    lexer_fsm_set_alnum_identifier(s + 1, 'n');
    lexer_fsm_add_edge(s + 2, lexer_fsm_get_char_index('e'), s + 3);
    lexer_fsm_set_alnum_identifier(s + 2, 'e');
    lexer_fsm_set_alnum_identifier(s + 3, 0);

    // - Double State Symbols
    // -- = & == -> 2 states
    s += 4;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('='), s);
    // --- states
    lexer_fsm_add_state(s, Token_Assignment); // =
    lexer_fsm_add_state(s + 1, Token_Equal); // ==
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('='), s + 1);

    // -- ! & != -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('!'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Not); //_ !
    lexer_fsm_add_state(s + 1, Token_Not_Equal); //_ !=
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('='), s + 1);

    // -- > & >= -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('>'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Bigger); // >
    lexer_fsm_add_state(s + 1, Token_Bigger_Equal); // >=
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('='), s + 1);

    // -- < & <= -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('<'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Smaller); // <
    lexer_fsm_add_state(s + 1, Token_Smaller_Equal); // <=
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('='), s + 1);

    // -- | & || -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('|'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Error); // |
    lexer_fsm_add_state(s + 1, Token_Or); // ||
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('|'), s + 1);

    // -- & & && -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('&'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Error); // &
    lexer_fsm_add_state(s + 1, Token_And); // &&
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index('&'), s + 1);

    // -- : & :) -> 2 states
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index(':'), s);
    // --- states
    lexer_fsm_add_state(s, Token_Colon); // :
    lexer_fsm_add_state(s + 1, Token_Smiley); // :)
    // --- edges
    lexer_fsm_add_edge(s, lexer_fsm_get_char_index(')'), s + 1);

    // - Single state symbols
    s += 2;
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('+'), s);
    lexer_fsm_add_state(s++, Token_Plus); // +
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('-'), s);
    lexer_fsm_add_state(s++, Token_Minus); // -
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('*'), s);
    lexer_fsm_add_state(s++, Token_Multiply); // *
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('/'), s);
    lexer_fsm_add_state(s++, Token_Divide); // /
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('%'), s);
    lexer_fsm_add_state(s++, Token_Modulu); // %
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index('('), s);
    lexer_fsm_add_state(s++, Token_Open_Paren); // (
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index(')'), s);
    lexer_fsm_add_state(s++, Token_Close_Paren); // )
    lexer_fsm_add_starting_state_index(lexer_fsm_get_char_index(';'), s);
    lexer_fsm_add_state(s++, Token_Semi_Colon); // ;
}
