#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dynarray.h"
#include "cmd.h"
#include "syntacticAnalysis.h"

enum {MAX_LINE_SIZE = 1024};
enum {FALSE, TRUE};

/* Check whether functional operator that have occur right before is in the right syntax.
   If the syntax is valid, returns 1(TRUE), and if it's invalid, returns 0(FALSE) */
int checkValid(int *nI, int *nLB, int *nRB, int add_nI, int add_nLB, int add_nRB, const char *programName){
    /* 'add_* == 1' means * occur
       'add_* == 0' means * did not occur */
    
    // printf("I(%d) LB(%d) RB(%d) <- add %d %d %d\n", *nI, *nLB, *nRB, add_nI, add_nLB, add_nRB);
    if(add_nI){
        if(*nRB) {
            fprintf(stderr, "%s: Multiple redirection of standard out\n", programName);
            return FALSE;
        }
        (*nI)++;
        *nLB=0;
        *nRB=0;
        return TRUE;
    }

    else if(add_nLB){
        if((*nI)==0) {
            if(*nLB){
                fprintf(stderr, "%s: Multiple redirection of standard input\n", programName);
                return FALSE;
            }
            (*nLB)++;
            return TRUE;
        }
        fprintf(stderr, "%s: Multiple redirection of standard input\n", programName);
        return FALSE;
    }

    else if(add_nRB){
        if(*nRB){
            fprintf(stderr, "%s: Multiple redirection of standard out\n", programName);
            return FALSE;
        }
        (*nRB)++;
        return TRUE;
    }

    else assert(FALSE);
}


/* synTokens checks whether oTokens have valid order of
   pipe, <, >, and other commadns. If vaild, it returns 1(TRUE),
   and if invalid, it returns 0(FALSE) */
int synTokens(DynArray_T oTokens, DynArray_T oCmds, const char *programName){
    assert(oTokens);
    assert(oCmds);
    
    enum SynState {STATE_START, STATE_NONFUNC, STATE_I, STATE_RB, STATE_LB};
    enum SynState eState = STATE_START;

    Token_T psToken;
    Cmd_T psCmd;
    enum TokenType nowType;
    enum CmdType cmdType=CMD_NONFUNC;
    char * nowValue;
    int nToken = DynArray_getLength(oTokens);
    if(nToken==0) return TRUE;
    int arglen=0;
    char *argv[MAX_LINE_SIZE];
    int i;
    int nI=0;
    int nLB=0;
    int nRB=0;

    for(i=0;i<=nToken;i++){
        /* Check whether Cmd ends with | or < or > */
        if(i==nToken){
            if(eState==STATE_RB){
                fprintf(stderr, "%s: Standard output redirection without file name\n", programName);
                return FALSE;
            }
            else if(eState==STATE_LB){
                fprintf(stderr, "%s: Standard input redirection without file name\n", programName);
                return FALSE;
            }
            else if(eState==STATE_I){
                fprintf(stderr, "%s: Pipe or redirection destination is not specified\n", programName);
                return FALSE;
            }
            else if(eState==STATE_NONFUNC){
                if(!(psCmd = makeCmd(arglen, argv, cmdType))){
                    fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                    return FALSE;
                }
                if (!DynArray_add(oCmds, psCmd)){
                    fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                    return FALSE;
                }
                return TRUE;
            }
            else assert(FALSE);
        }

        psToken = (Token_T)DynArray_get(oTokens, i);
        nowType = psToken->eType;
        nowValue = psToken->pcValue;

        switch (eState){
        case STATE_START:
            if(nowType==TOKEN_NON_FUNCTIONAL){
                argv[arglen++]=nowValue;
                eState=STATE_NONFUNC;
            }
            else if((nowType==TOKEN_FUNCTIONAL_I) || (nowType==TOKEN_FUNCTIONAL_RB) || (nowType==TOKEN_FUNCTIONAL_LB)){
                fprintf(stderr, "%s: Pipe or redirection destination is not specified\n", programName);
                return FALSE;
            }
            else assert(FALSE);
            break;

        case STATE_NONFUNC:
            if(nowType==TOKEN_NON_FUNCTIONAL){
                argv[arglen++]=nowValue;
                eState=STATE_NONFUNC;
            }
            else if(nowType==TOKEN_FUNCTIONAL_I){
                if(!checkValid(&nI, &nLB, &nRB, TRUE, FALSE, FALSE, programName))
                    return FALSE;

                // Add Cmd for before |
                if(!(psCmd = makeCmd(arglen, argv, cmdType))){
                    fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                    return FALSE;
                }
                if (!DynArray_add(oCmds, psCmd)){
                    fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                    return FALSE;
                }
                // // Add | Cmd
                // arglen=1;
                // argv[0]=nowValue;
                // if(!(psCmd = makeCmd(arglen, argv, CMD_I))){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // if (!DynArray_add(oCmds, psCmd)){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                cmdType=CMD_NONFUNC;
                arglen=0;
                eState=STATE_I;
            }
            else if(nowType==TOKEN_FUNCTIONAL_RB){
                if(!checkValid(&nI, &nLB, &nRB, FALSE, FALSE, TRUE, programName))
                    return FALSE;

                // // Add Cmd for before >
                // if(!(psCmd = makeCmd(arglen, argv, CMD_NONFUNC))){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // if (!DynArray_add(oCmds, psCmd)){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }                
                // // Add > Cmd
                // arglen=1;
                // argv[0]=nowValue;
                // if(!(psCmd = makeCmd(arglen, argv, CMD_RB))){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // if (!DynArray_add(oCmds, psCmd)){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // arglen=0;
                argv[arglen++]=nowValue;
                cmdType=CMD_RB;
                eState=STATE_RB;
            }
            else if(nowType==TOKEN_FUNCTIONAL_LB){
                if(!checkValid(&nI, &nLB, &nRB, FALSE, TRUE, FALSE, programName))
                    return FALSE;

                // // Add Cmd for before <
                // if(!(psCmd = makeCmd(arglen, argv, CMD_NONFUNC))){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // if (!DynArray_add(oCmds, psCmd)){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }                
                // // Add < Cmd
                // arglen=1;
                // argv[0]=nowValue;
                // if(!(psCmd = makeCmd(arglen, argv, CMD_LB))){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // if (!DynArray_add(oCmds, psCmd)){
                //     fprintf(stderr, "%s: Cannot allocate memory\n", programName);
                //     return FALSE;
                // }
                // arglen=0;
                argv[arglen++]=nowValue;
                cmdType=CMD_LB;
                eState=STATE_LB;
            }
            else assert(FALSE);
            break;
        
        case STATE_I:
            if(nowType==TOKEN_NON_FUNCTIONAL){
                argv[arglen++]=nowValue;
                eState=STATE_NONFUNC;
            }
            else if((nowType==TOKEN_FUNCTIONAL_I) || (nowType==TOKEN_FUNCTIONAL_RB) || (nowType==TOKEN_FUNCTIONAL_LB)){
                fprintf(stderr, "%s: Pipe or redirection destination is not specified\n", programName);
                return FALSE;
            }
            else assert(FALSE);
            break;
        
        case STATE_RB:
            if(nowType==TOKEN_NON_FUNCTIONAL){
                argv[arglen++]=nowValue;
                eState=STATE_NONFUNC;
            }
            else if((nowType==TOKEN_FUNCTIONAL_I) || (nowType==TOKEN_FUNCTIONAL_RB) || (nowType==TOKEN_FUNCTIONAL_LB)){
                fprintf(stderr, "%s: Standard output redirection without file name\n", programName);
                return FALSE;
            }
            else assert(FALSE);
            break;

        case STATE_LB:
            if(nowType==TOKEN_NON_FUNCTIONAL){
                argv[arglen++]=nowValue;
                eState=STATE_NONFUNC;
            }
            else if((nowType==TOKEN_FUNCTIONAL_I) || (nowType==TOKEN_FUNCTIONAL_RB) || (nowType==TOKEN_FUNCTIONAL_LB)){
                fprintf(stderr, "%s: Standard input redirection without file name\n", programName);
                return FALSE;
            }
            else assert(FALSE);
            break;
        
        default:
            break;
        }
    }
    assert(FALSE);
}