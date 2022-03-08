#pragma once

// Change printf colors
void red();
void cyan();
void reset();

// Reports an error message to the user and exits
void error_handler_report(int line, char* msg, int argc, void* argv[]);

// Reports an allocation error message to the user and exits
void error_handler_report_alloc();
