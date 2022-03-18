#pragma once

#include "compiler/compiler.h"


/* ---------- Global variables ---------- */

// In that file all the global variables are extern, so every file that includes them 
// will be able to use them but not to define them.
// The global variables should only be defined once in the whole project,
// that definition is in the global.c file.

// The global project compiler
extern Compiler compiler;
