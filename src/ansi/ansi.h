#pragma once


// The ansi.h and ansi.c files were created to add support for the ANSI escape codes.
// This enables printing in different colors in the console.
// Taken from here: https://youtu.be/bQ8qaBjJtYU

/* ---------- Functions ---------- */

// Set up the console to support the ANSI escape codes,
// so we can print in different colors in the console.
void ansi_setup_console();
