#pragma once

#include "compiler/compiler.h"


/* ---------- Global variables ---------- */

// For each global variable I've defined a guard macro to help me identify if that variable is defined in that file or in another.
// A file that wants to define that variable will define that guard macro causing the declaration to be regular.
// For every other file that just wants to use that variable, the declaration will be extern.
// Each guard macro should only be defined once in the whole project, to make sure there will be no variable definition collisions.

// A global variable for the entire compiler
#ifndef COMPILER_DEFINITION
extern Compiler compiler;
#else
Compiler compiler;
#endif
