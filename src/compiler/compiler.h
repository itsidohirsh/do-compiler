#pragma once

#include "../scope_tree/scope_tree.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"


/* ---------- Structs ---------- */

// Struct of the entire compile
typedef struct Compiler
{
    // The source code to be compiled
    char* src;
    // The symbol table of the compiler
    Scope_Tree* scope_tree;
    // The lexer of the compiler
    Lexer* lexer;
    // The parser of the compiler
    Parser* parser;
} Compiler;


/* ---------- Functions ---------- */

// Gets the source code, and initializes the global compiler with all it's necessary components
void compiler_init(char* src);

// Destroys all the components of the global compiler
void compiler_destroy();

// Compiles the source code
void compiler_compile();
