#ifndef PROCESSEACHLINE_H
#define PROCESSEACHLINE_H

#include "dynarray.h"

/* Completely free Dynarray which consists of tokens. */
void Dynarray_tokens_free(DynArray_T oDynarray);

/* Completely free Dynarray which consists of cmds. */
void Dynarray_cmds_free(DynArray_T oDynarray);

/* Do overall process of pcLine.
   1) If the pcLine is come from .ishrc, it prints pcLine
   2) Do lexical analysis in order to determine each token
   3) Do syntactic analysis in order to check the syntatic is correct
      in the view of pipe
   4) Execute pcLine as command
   In each step, error could occurr. If error occurr, it prints out
   error message to stderr stream and returns FALSE(0).
   If the hole process has go through well, it returns TRUE(1). */
int processEachLine(int doPrint, char *pcLine, const char *programName);

#endif