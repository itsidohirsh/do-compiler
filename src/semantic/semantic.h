#pragma once


/* ---------- Functions ---------- */

// Checks after reduction by the production rule: DECL -> data_type id ; (R7)
// if the identifier is decleared in the current scope or not.
// If not inserts a new entry into the current scope's symbol with the known data.
// If yes, reports a semantic error of already exists.
void semantic_decl();
