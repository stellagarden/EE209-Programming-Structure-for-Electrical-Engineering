#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"

/* Make new Cmd structure consist of arglen, argv, cmdType
   arglen: lenght of argv
   argv: arguments that this command have
   cmdType: indicates that this cmd is normal command or > or < or | */
Cmd_T makeCmd(int arglen, char **argv, enum CmdType cmdType){
    Cmd_T psCmd;
    if(!(psCmd=(Cmd_T)malloc(sizeof(struct Cmd))))
        return NULL;
    
    if(!(psCmd->cmdName=(char*)calloc(strlen(argv[0])+1, sizeof(char)))){
        free(psCmd);
        return NULL;
    }
    strcpy(psCmd->cmdName, argv[0]);

    if(!(psCmd->argv=(char**)calloc(arglen, sizeof(char *)))){
        free(psCmd->cmdName);
        free(psCmd);
        return NULL;
    }    
    for(int i=0;i<arglen;i++){
        if(!(psCmd->argv[i]=calloc(strlen(argv[i])+1, sizeof(char)))){
            free(psCmd->cmdName);
            for(i=i-1;i>=0;i--){
                free(psCmd->argv[i]);
            }
            free(psCmd);
        }
        strcpy(psCmd->argv[i],argv[i]);
    }

    psCmd->arglen=arglen;
    psCmd->cmdType=cmdType;
    return psCmd;
}

/* Free cmd. pvExtra is unused */
void freeCmd(void *pvItem, void *pvExtra){
    assert(pvItem);

    Cmd_T psCmd = (Cmd_T)pvItem;
    free(psCmd->cmdName);
    for(int i=0;i<psCmd->arglen;i++){
        free(psCmd->argv[i]);
    }
    free(psCmd);
}

/* Print cmd. pvExtra is unused */
void printCmd(void *pvItem, void *pvExtra){
    assert(pvItem);

    Cmd_T psCmd = (Cmd_T)pvItem;
    printf("(%d) <%s: %d> {", psCmd->cmdType, psCmd->cmdName, psCmd->arglen);
    for(int i=0;i<psCmd->arglen;i++){
        printf("%s, ", psCmd->argv[i]);
    }
    printf("}\n");
}