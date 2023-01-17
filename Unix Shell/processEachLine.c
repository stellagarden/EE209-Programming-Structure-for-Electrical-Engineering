#include <stdio.h>
#include <assert.h>

#include "processEachLine.h"
#include "dynarray.h"
#include "cmd.h"
#include "lexicalAnalysis.h"
#include "syntacticAnalysis.h"
#include "execute.h"

enum {FALSE, TRUE};

#define checkLex FALSE
#define checkSyn FALSE

/* Completely free Dynarray which consists of tokens. */
void Dynarray_tokens_free(DynArray_T oDynarray){
    DynArray_map(oDynarray, freeToken, NULL);
    DynArray_free(oDynarray);
}

/* Completely free Dynarray which consists of cmds. */
void Dynarray_cmds_free(DynArray_T oDynarray){
    DynArray_map(oDynarray, freeCmd, NULL);
    DynArray_free(oDynarray);
}

/* Do overall process of pcLine.
   1) If the pcLine is come from .ishrc, it prints pcLine
   2) Do lexical analysis in order to determine each token
   3) Do syntactic analysis in order to check the syntatic is correct
      in the view of pipe
   4) Execute pcLine as command
   In each step, error could occurr. If error occurr, it prints out
   error message to stderr stream and returns FALSE(0).
   If the hole process has go through well, it returns TRUE(1). */
int processEachLine(int doPrint, char *pcLine, const char *programName){
    assert(pcLine);
    assert(programName);

    DynArray_T oTokens = DynArray_new(0);
    DynArray_T oCmds = DynArray_new(0);

    // 1) If the pcLine is come from .ishrc, it prints pcLine
    if(doPrint) {
        printf("%s", pcLine);
    }
    
    // 2) Lexical Analysis
    if(!lexLine(pcLine, oTokens, programName)){
        printf("%% ");
        Dynarray_tokens_free(oTokens);
        Dynarray_cmds_free(oCmds);
        return FALSE;
    }
    if(checkLex) DynArray_map(oTokens, printToken, NULL);

    // 3) Syntactic Analysis
    if(!synTokens(oTokens, oCmds, programName)){
        printf("%% ");
        Dynarray_tokens_free(oTokens);
        Dynarray_cmds_free(oCmds);
        return FALSE;
    }
    if(checkSyn) DynArray_map(oCmds, printCmd, NULL);

    // 4) Execution
    if(!exeCmds(oCmds, programName)){
        printf("%% ");
        Dynarray_tokens_free(oTokens);
        Dynarray_cmds_free(oCmds);
        return FALSE;
    }

    printf("%% ");
    Dynarray_tokens_free(oTokens);
    Dynarray_cmds_free(oCmds);
    return TRUE;
}

