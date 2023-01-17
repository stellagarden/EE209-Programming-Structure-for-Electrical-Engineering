/*--------------------------------------------------------------------*/
/* lexicalAnalysis.c                                                  */
/* Original Author: Bob Dondero                                       */
/* Illustrate lexical analysis using a deterministic finite state     */
/* automaton (DFA) - Modified by Hyemin Lee (2020-12-19)              */
/*--------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dynarray.h"
#include "lexicalAnalysis.h"

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/
/* Free token pvItem. pvExtra is unused */
void freeToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   free(psToken->pcValue);
   free(psToken);
}
/*--------------------------------------------------------------------*/
/* Print token pvItem to stdout iff it is a not functional.
   pvExtra is unused */
void printNonFunctionalToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   if (psToken->eType == TOKEN_NON_FUNCTIONAL)
      printf("%s, ", psToken->pcValue);
}
/*--------------------------------------------------------------------*/
/* Print token pvItem to stdout iff it is a functional |.
   pvExtra is unused */
void printFunctionalIToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   if (psToken->eType == TOKEN_FUNCTIONAL_I)
      printf("%s, ", psToken->pcValue);
}
/*--------------------------------------------------------------------*/
/* Print token pvItem to stdout iff it is a functional >.
   pvExtra is unused */
void printFunctionalRBToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   if (psToken->eType == TOKEN_FUNCTIONAL_RB)
      printf("%s, ", psToken->pcValue);
}
/*--------------------------------------------------------------------*/
/* Print token pvItem to stdout iff it is a functional <.
   pvExtra is unused */
void printFunctionalLBToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   if (psToken->eType == TOKEN_FUNCTIONAL_LB)
      printf("%s, ", psToken->pcValue);
}
/*--------------------------------------------------------------------*/
/* Print token information to stdout. pvExtra is unused */
void printToken(void *pvItem, void *pvExtra)
{
   assert(pvItem);

   Token_T psToken = (Token_T)pvItem;
   if (psToken->eType == TOKEN_NON_FUNCTIONAL)
      printf("Non-func : %s\n", psToken->pcValue);
   else if(psToken->eType == TOKEN_FUNCTIONAL_I)
      printf("Func |   : %s\n", psToken->pcValue);
   else if(psToken->eType == TOKEN_FUNCTIONAL_RB)
      printf("Func >   : %s\n", psToken->pcValue);
   else if(psToken->eType == TOKEN_FUNCTIONAL_LB)
      printf("Func <   : %s\n", psToken->pcValue);
   else assert(FALSE);
}
/*--------------------------------------------------------------------*/
/* Create and return a Token whose type is eTokenType and whose
   value consists of string pcValue.  Return NULL if insufficient
   memory is available.  The caller owns the Token. */
Token_T makeToken(enum TokenType eTokenType,
   char *pcValue)
{
   Token_T psToken;

   psToken = (Token_T)malloc(sizeof(struct Token));
   if (psToken == NULL)
      return NULL;

   psToken->eType = eTokenType;

   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
   {
      free(psToken);
      return NULL;
   }

   strcpy(psToken->pcValue, pcValue);

   return psToken;
}
/*--------------------------------------------------------------------*/
/* Lexically analyze string pcLine.  Populate oTokens with the
   tokens that pcLine contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oTokens may contain
   tokens that were discovered before the error. The caller owns the
   tokens placed in oTokens. */
   
/* lexLine() uses a DFA approach.  It "reads" its characters from
   pcLine. */
int lexLine(const char *pcLine, DynArray_T oTokens, const char *programName)
{
   enum LexState {STATE_START, STATE_WORD, STATE_QUOTE_S, STATE_QUOTE_L};

   enum LexState eState = STATE_START;

   int iLineIndex = 0;
   int iValueIndex = 0;
   char c;
   char acValue[MAX_LINE_SIZE];
   Token_T psToken;

   assert(pcLine);
   assert(oTokens);

   for (;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];

      switch (eState)
      {
         case STATE_START:
            if ((c == '\n') || (c == '\0') || (c == EOF))
               return TRUE;
            else if (isspace(c))
            {
               eState = STATE_START;
            }
            else if (c == '|')
            {
               /* Create a FUNCTIONAL | token. */
               acValue[iValueIndex++] = '|';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_I, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c == '\''){
               eState = STATE_QUOTE_S;
            }
            else if (c == '"'){
               eState = STATE_QUOTE_L;
            }
            else if (c == '>'){
               /* Create a FUNCTIONAL > token. */
               acValue[iValueIndex++] = '>';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_RB, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c == '<'){
               /* Create a FUNCTIONAL > token. */
               acValue[iValueIndex++] = '<';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_LB, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else  // Ordinary command
            {
               acValue[iValueIndex++] = c;
               eState = STATE_WORD;
            }
            break;

         case STATE_WORD:
            if ((c == '\n') || (c == '\0') || (c == EOF))
            {
               /* Create a ordinary token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NON_FUNCTIONAL, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               return TRUE;
            }
            else if (isspace(c))
            {
               /* Create a ordinary token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NON_FUNCTIONAL, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c == '|')
            {
               /* Create a ordinary token with FUNCTIONAL | token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NON_FUNCTIONAL, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               acValue[iValueIndex++] = '|';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_I, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c == '\''){
               eState = STATE_QUOTE_S;
            }
            else if (c == '"'){
               eState = STATE_QUOTE_L;
            }
            else if (c == '>')
            {
               /* Create a ordinary token with FUNCTIONAL > token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NON_FUNCTIONAL, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               acValue[iValueIndex++] = '>';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_RB, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c == '<')
            {
               /* Create a ordinary token with FUNCTIONAL | token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NON_FUNCTIONAL, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               acValue[iValueIndex++] = '<';
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_FUNCTIONAL_LB, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else  // Ordinary command
            {
               acValue[iValueIndex++] = c;
               eState = STATE_WORD;
            }
            break;

         case STATE_QUOTE_S:
            if ((c == '\n') || (c == '\0') || (c == EOF)){
               fprintf(stderr, "./ish: Could not find quote pair\n");
               return FALSE;
            }
            else if (c == '\''){
               eState = STATE_WORD;
            }
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_QUOTE_S;
            }
            break;
         
         case STATE_QUOTE_L:
            if ((c == '\n') || (c == '\0') || (c == EOF)){
               fprintf(stderr, "./ish: Could not find quote pair\n");
               return FALSE;
            }
            else if (c == '"'){
               eState = STATE_WORD;
            }
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_QUOTE_L;
            }
            break;

         default:
            assert(FALSE);
      }
   }
}
/*--------------------------------------------------------------------*/
