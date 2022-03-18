// This file is where I define the guard macros for all the global variables of the project.
// Each guard macro should only be defined once in the whole project.

// Defining COMPILER_DEFINITION so the compiler will be decleared regularly and not extern.
#define COMPILER_DEFINITION


// Include global.h after defining the guard macros,
// so the all the global variables declarations will be declared regularly and not extern.
#include "global.h"
