#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/lexer_fsm.h"

void lexer_fsm_init(Lexer_FSM* fsm)
{
    // Variable to create current state index for each state in the FSM of the lexer
    int s = 1;

    // - Whitespace -> 1
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index(' '), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\t'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\r'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\n'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\v'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\f'), s);
    // -- states
    lexer_fsm_add_state(fsm, s, Token_Whitespace);
    // -- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index(' '), s);
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('\t'), s);
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('\r'), s);
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('\n'), s);
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('\v'), s);
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('\f'), s);

    // - Identifier -> 2
    s = 2;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('_'), s);
    int i;
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        if (isalpha(i))
            lexer_fsm_add_state_index(fsm, i, s);
    // -- states
    lexer_fsm_add_state(fsm, s, Token_Identifier);
    // -- edges
    lexer_fsm_set_alnum_identifier(fsm, s, 0);

    // - Number literal -> 3
    s = 3;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('0'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('1'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('2'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('3'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('4'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('5'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('6'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('7'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('8'), s);
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('9'), s);
    // -- states
    lexer_fsm_add_state(fsm, s, Token_Number);
    // -- edges
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        if (isdigit(i))
            lexer_fsm_add_edge(fsm, s, i, s);

    // - Character literal -> 4, 5, 6
    s = 4;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('\''), s);
    // -- states
    lexer_fsm_add_state(fsm, s, Token_Error); // '
    lexer_fsm_add_state(fsm, s + 1, Token_Error); // 'C
    lexer_fsm_add_state(fsm, s + 2, Token_Character); // 'C'
    // -- edges
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        lexer_fsm_add_edge(fsm, s, i, s + 1);
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('\''), s + 2);

    // - Keywords
    // -- "int" & "if" -> 7, 8, 9, 10
    s = 7;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('i'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // i
    lexer_fsm_add_state(fsm, s + 1, Token_If); // if
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // in
    lexer_fsm_add_state(fsm, s + 3, Token_Int); // int
    // --- edges
    // ---- "int"
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('n'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s, 'n');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('t'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 't');
    lexer_fsm_set_alnum_identifier(fsm, s + 3, 0);
    // ---- "if"
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('f'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 0);

    // -- "char" -> 11, 12, 13, 14
    s = 11;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('c'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // c
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // ch
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // cha
    lexer_fsm_add_state(fsm, s + 3, Token_Char); // char
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('h'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'h');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('a'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 'a');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('r'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 'r');
    lexer_fsm_set_alnum_identifier(fsm, s + 3, 0);

    // -- "prog" -> 15, 16, 17, 18
    s = 15;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('p'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // p
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // pr
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // pro
    lexer_fsm_add_state(fsm, s + 3, Token_Prog); // prog
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('r'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'r');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('o'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 'o');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('g'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 'g');
    lexer_fsm_set_alnum_identifier(fsm, s + 3, 0);

    // -- "else" -> 19, 20, 21, 22
    s = 19;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('e'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // e
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // el
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // els
    lexer_fsm_add_state(fsm, s + 3, Token_Else); // else
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('l'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'l');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('s'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 's');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('e'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 'e');
    lexer_fsm_set_alnum_identifier(fsm, s + 3, 0);

    // -- "while" -> 23, 24, 25, 26, 27
    s = 23;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('w'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // w
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // wh
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // whi
    lexer_fsm_add_state(fsm, s + 3, Token_Identifier); // whil
    lexer_fsm_add_state(fsm, s + 4, Token_While); // while
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('h'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'h');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('i'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 'i');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('l'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 'l');
    lexer_fsm_add_edge(fsm, s + 3, lexer_fsm_get_char_index('e'), s + 4);
    lexer_fsm_set_alnum_identifier(fsm, s + 4, 'e');
    lexer_fsm_set_alnum_identifier(fsm, s + 5, 0);

    // -- "set" -> 28, 29, 30
    s = 28;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('s'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // s
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // se
    lexer_fsm_add_state(fsm, s + 2, Token_Set); // set
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('e'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'e');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('t'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 't');
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 0);

    // -- "done" -> 31, 32, 33, 34
    s = 31;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('d'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Identifier); // d
    lexer_fsm_add_state(fsm, s + 1, Token_Identifier); // do
    lexer_fsm_add_state(fsm, s + 2, Token_Identifier); // don
    lexer_fsm_add_state(fsm, s + 3, Token_Done); // done
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('o'), s + 1);
    lexer_fsm_set_alnum_identifier(fsm, s, 'o');
    lexer_fsm_add_edge(fsm, s + 1, lexer_fsm_get_char_index('n'), s + 2);
    lexer_fsm_set_alnum_identifier(fsm, s + 1, 'n');
    lexer_fsm_add_edge(fsm, s + 2, lexer_fsm_get_char_index('e'), s + 3);
    lexer_fsm_set_alnum_identifier(fsm, s + 2, 'e');
    lexer_fsm_set_alnum_identifier(fsm, s + 3, 0);

    // - Double State Symbols
    // -- = & ==
    s = 35;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('='), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Assignment); // =
    lexer_fsm_add_state(fsm, s + 1, Token_Equal); // ==
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('='), s + 1);

    // -- ! & !=
    s = 37;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('!'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Not); //_ !
    lexer_fsm_add_state(fsm, s + 1, Token_Not_Equal); //_ !=
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('='), s + 1);

    // -- > & >=
    s = 39;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('>'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Bigger); // >
    lexer_fsm_add_state(fsm, s + 1, Token_Bigger_Equal); // >=
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('='), s + 1);

    // -- < & <=
    s = 41;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('<'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Smaller); // <
    lexer_fsm_add_state(fsm, s + 1, Token_Smaller_Equal); // <=
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('='), s + 1);

    // -- | & ||
    s = 43;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('|'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Error); // |
    lexer_fsm_add_state(fsm, s + 1, Token_Or); // ||
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('|'), s + 1);

    // -- & & &&
    s = 45;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('&'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Error); // &
    lexer_fsm_add_state(fsm, s + 1, Token_And); // &&
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index('&'), s + 1);

    // -- : & :)
    s = 47;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index(':'), s);
    // --- states
    lexer_fsm_add_state(fsm, s, Token_Colon); // :
    lexer_fsm_add_state(fsm, s + 1, Token_Smiley); // :)
    // --- edges
    lexer_fsm_add_edge(fsm, s, lexer_fsm_get_char_index(')'), s + 1);

    // - Single state symbols
    s = 49;
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('+'), s);
    lexer_fsm_add_state(fsm, s++, Token_Plus); // +
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('-'), s);
    lexer_fsm_add_state(fsm, s++, Token_Minus); // -
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('*'), s);
    lexer_fsm_add_state(fsm, s++, Token_Multiply); // *
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('/'), s);
    lexer_fsm_add_state(fsm, s++, Token_Divide); // /
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('%'), s);
    lexer_fsm_add_state(fsm, s++, Token_Modulu); // %
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index('('), s);
    lexer_fsm_add_state(fsm, s++, Token_Open_Paren); // (
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index(')'), s);
    lexer_fsm_add_state(fsm, s++, Token_Close_Paren); // )
    lexer_fsm_add_state_index(fsm, lexer_fsm_get_char_index(';'), s);
    lexer_fsm_add_state(fsm, s++, Token_Semi_Colon); // ;
}

void lexer_fsm_set_alnum_identifier(Lexer_FSM* fsm, int state_index, char except)
{
    int i;
    for (i = 0; i < NUM_OF_CHARACTERS; i++)
        if ((isalnum(i) || i == '_') && i != except)
            lexer_fsm_add_edge(fsm, state_index, i, lexer_fsm_get_state_index(fsm, '_'));
}

int lexer_fsm_get_char_index(char value)
{
    // If the character is out of range return the first index in the array, 0
    // Else reutrn it's proper index
    return (value >= 0) ? value : 0;
}

int lexer_fsm_get_state_index(Lexer_FSM* fsm, char value)
{
    return fsm->starting_state_indices[lexer_fsm_get_char_index(value)];
}

void lexer_fsm_add_state_index(Lexer_FSM* fsm, int char_index, int starting_state_index)
{
    fsm->starting_state_indices[char_index] = starting_state_index;
}

void lexer_fsm_add_state(Lexer_FSM* fsm, int state_number, Token_Type token_type)
{
    fsm->states[state_number].token_type = token_type;
}

void lexer_fsm_add_edge(Lexer_FSM* fsm, int from, int to, char state_number)
{
    fsm->edges[from][to].state_number = state_number;
}

bool lexer_fsm_is_adjacent(Lexer_FSM* fsm, int from, int to)
{
    return fsm->edges[from][to].state_number > 0;
}

void lexer_fsm_print(Lexer_FSM* fsm)
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

    for (i = 0; i < NUM_OF_STATES; i++)
    {
        printf("%2d. | ", i);

        for (j = 0; j < NUM_OF_CHARACTERS; j++)
        {
            if (fsm->edges[i][j].state_number != 0)
                printf("%d", fsm->edges[i][j].state_number);
            else
                printf("%c", '.');
        }
        printf("\n");
    }
    printf("\n");
}
