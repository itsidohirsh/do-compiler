// int i;

// // Identifier
// lexer_fsm_add_state(fsm, IDENTIFIER_INDEX, ' ', Token_Identifier, true);
// for (i = 0; i < NUM_STATES; i++)
//     if (isalnum(i + LOWER_BOUND))
//         lexer_fsm_add_edge(fsm, IDENTIFIER_INDEX, i, IDENTIFIER_INDEX);

// // "prog"
// lexer_fsm_add_state(fsm, lexer_fsm_get_index('p'), 'p', Token_Identifier, false);
// lexer_fsm_add_state(fsm, lexer_fsm_get_index('r'), 'r', Token_Identifier, false);
// lexer_fsm_add_state(fsm, lexer_fsm_get_index('o'), 'o', Token_Identifier, false);
// lexer_fsm_add_state(fsm, lexer_fsm_get_index('g'), 'g', Token_Prog, true);
// lexer_fsm_add_edge(fsm, lexer_fsm_get_index('p'), lexer_fsm_get_index('r'), lexer_fsm_get_index('r'));
// lexer_fsm_add_edge(fsm, lexer_fsm_get_index('r'), lexer_fsm_get_index('o'), lexer_fsm_get_index('o'));
// lexer_fsm_add_edge(fsm, lexer_fsm_get_index('o'), lexer_fsm_get_index('g'), lexer_fsm_get_index('g'));

// // = & ==
// lexer_fsm_add_state(fsm, lexer_fsm_get_index('='), '=', Token_Assignment, true);
// lexer_fsm_add_edge(fsm, lexer_fsm_get_index('='), lexer_fsm_get_index('='), lexer_fsm_get_index('='));

/////////////////////////////////////////////////////
// Lexer_Edge* lexer_fsm_get_edge(int from, int to);
// Lexer_State* lexer_fsm_get_state(int index);

/////////////////////////////////////////////////////
// Token* lexer_next_token(Lexer* lexer)
// {
//     // i = index of the current character in the source code
//     int i = lexer_fsm_get_index(lexer->fsm, (Lexer_State) { .value = lexer->c, .token_type = Token_Error, .is_final_state = false });
//     printf("%d\n", i);
//     // value = the substring from the source code that is the current token value
//     char* value = (char*) calloc(32, sizeof(char));
//     // size of the value 
//     int size = 0;

//     // While not EOS
//     while (lexer->c != '\0')
//     {
//         // Update current token's value
//         value[size++] = lexer->c;

//         // If there is no edge between curren character and next character
//         if (lexer->fsm->edges[i][lexer_fsm_get_index(lexer->fsm, (Lexer_State) { .value = lexer->src[lexer->i + 1], .token_type = Token_Error, .is_final_state = false })].weight == 0)
//             return token_init(value, lexer->fsm->states[lexer_fsm_get_index(lexer->fsm, (Lexer_State) { .value = lexer->c, .token_type = Token_Error, .is_final_state = false })].token_type);

//         // Go to next state
//         i = lexer->fsm->edges[i][lexer_fsm_get_index(lexer->fsm, (Lexer_State) { .value = lexer->src[lexer->i + 1], .token_type = Token_Error, .is_final_state = false })].weight;
//         lexer_advance(lexer);
//     }

//     // If we are the the end of the source code but there is a value to the current token, return the token
//     if (size != 0)
//         return token_init(value, lexer->fsm->states[lexer_fsm_get_index(lexer->fsm, (Lexer_State) { .value = lexer->c, .token_type = Token_Error, .is_final_state = false })].token_type);

//     // Return EOF token
//     return token_init(0, Token_Eof);
// }