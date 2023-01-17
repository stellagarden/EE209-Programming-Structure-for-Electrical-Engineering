#ifndef SYNTACTICANALYSIS_H
#define SYNTACTICANALYSIS_H

#include "dynarray.h"
#include "cmd.h"

/* Check whether functional operator that have occur right before is in the right syntax.
   If the syntax is valid, returns 1(TRUE), and if it's invalid, returns 0(FALSE) */
int checkValid(int *nI, int *nLB, int *nRB,
   int add_nI, int add_nLB, int add_nRB, const char *programName);

/* synTokens checks whether oTokens have valid order of
   pipe, <, >, and other commadns. If vaild, it returns 1(TRUE),
   and if invalid, it returns 0(FALSE) */
int synTokens(DynArray_T oTokens, DynArray_T oCmds, const char *programName);

#endif