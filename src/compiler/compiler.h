#pragma once

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../scope_tree/scope_tree.h"
#include "../code_generator/code_generator.h"


/* ---------- Structs ---------- */

// Struct of the entire compile
typedef struct Compiler
{
    // The source code to be compiled
    char* src;
    // The name of the destination file
    char* dest_file_name;
    // The lexer of the compiler
    Lexer* lexer;
    // The parser of the compiler
    Parser* parser;
    // The scope tree of the compiler
    Scope_Tree* scope_tree;
    // The code generator of the compiler
    Code_Generator* code_generator;
    // Number of errors found during compilation
    int errors;
    // Current line number in source file for error reporting
    int line;
} Compiler;


/* ---------- Functions ---------- */

// Gets the source code file name and destination file name, and initializes the global compiler with all it's necessary components
void compiler_init(char* src_file_name, char* dest_file_name);

// Destroys all the components of the global compiler
void compiler_destroy();

// Compiles the source code
void compiler_compile();
