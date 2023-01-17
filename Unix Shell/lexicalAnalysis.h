/*--------------------------------------------------------------------*/
/* lexicalAnalysis.h                                                  */
/* Original Author: Bob Dondero                                       */
/* Modified by Hyemin Lee (2020-12-19)                                */
/*--------------------------------------------------------------------*/

#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include "dynarray.h"

/* Free token pvItem. pvExtra is unused */
void freeToken(void *pvItem, void *pvExtra);

/* Print token pvItem to stdout iff it is a not functional.
   pvExtra is unused */
void printNonFunctionalToken(void *pvItem, void *pvExtra);

/* Print token pvItem to stdout iff it is a functional |.
   pvExtra is unused */
void printFunctionalIToken(void *pvItem, void *pvExtra);

/* Print token pvItem to stdout iff it is a functional >.
   pvExtra is unused */
void printFunctionalRBToken(void *pvItem, void *pvExtra);

/* Print token pvItem to stdout iff it is a functional <.
   pvExtra is unused */
void printFunctionalLBToken(void *pvItem, void *pvExtra);

/* Print token information to stdout. pvExtra is unused */
void printToken(void *pvItem, void *pvExtra);

/* Create and return a Token whose type is eTokenType and whose
   value consists of string pcValue.  Return NULL if insufficient
   memory is available.  The caller owns the Token. */
Token_T makeToken(enum TokenType eTokenType,
   char *pcValue);

/* Lexically analyze string pcLine.  Populate oTokens with the
   tokens that pcLine contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oTokens may contain
   tokens that were discovered before the error. The caller owns the
   tokens placed in oTokens. */
   
/* lexLine() uses a DFA approach.  It "reads" its characters from
   pcLine. */
int lexLine(const char *pcLine, DynArray_T oTokens, const char *programName);

#endif