#pragma once

#include "../parser/parse_tree/parse_tree.h"


/* ---------- Structs ---------- */


/* ---------- Functions ---------- */

// Generates the assembly code for the given parse tree
void code_generator_generate(Parse_Tree_Node* parse_tree);

//
void code_generator_block(Parse_Tree_Node* block);

//
void code_generator_stmt(Parse_Tree_Node* stmt);

//
void code_generator_decl(Parse_Tree_Node* decl);

//
void code_generator_assign(Parse_Tree_Node* assign);

//
void code_generator_if_else(Parse_Tree_Node* if_else);

//
void code_generator_else(Parse_Tree_Node* _else);

//
void code_generator_while(Parse_Tree_Node* _while);

//
void code_generator_l_log_expr(Parse_Tree_Node* l_log_expr);

//
void code_generator_h_log_expr(Parse_Tree_Node* h_log_expr);

//
void code_generator_bool_expr(Parse_Tree_Node* bool_expr);

//
void code_generator_expr(Parse_Tree_Node* expr);

//
void code_generator_term(Parse_Tree_Node* term);

//
void code_generator_factor(Parse_Tree_Node* factor);
