#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "dynarray.h"
#include "execute.h"
#include "cmd.h"
#include "builtin.h"

enum {MAX_LINE_SIZE = 1024};
enum {FALSE, TRUE};

/* Execute non-Builtin commands */
int nonBuiltin(DynArray_T oCmds, int numCmds, const char *programName){
    int status=0;
    fflush(NULL);
    int pid = fork();
    // Child_1 : Program executing
    if(pid==0){
        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT,SIG_DFL);
        signal(SIGALRM,SIG_DFL);

        int pid2;
        int fds[2];
        Cmd_T psCmd2;
        for(int i=0; i<numCmds-1;i++){
            if(pipe(fds)==-1){
                fprintf(stderr, "%s: pipe building fail\n", programName);
                exit(-1);
            }

            fflush(NULL);
            pid2=fork();
            // Child_2 : stdout to Child_1
            if(!pid2){
                close(fds[0]);
                dup2(fds[1], 1);
                close(fds[1]);
                psCmd2 = (Cmd_T)DynArray_get(oCmds, i);
                execvp((psCmd2->argv)[0], (psCmd2->argv));
                perror((psCmd2->argv)[0]);
                exit(-1);
            }
            // Child_1 : stdin from Child_2
            else{
                close(fds[1]);
                dup2(fds[0], 0);
                close(fds[0]);
                wait(&status);
            }
        }
        psCmd2 = (Cmd_T)DynArray_get(oCmds, numCmds-1);
        execvp((psCmd2->argv)[0], (psCmd2->argv));
        perror((psCmd2->argv)[0]);
        exit(-1);

    }
    // Parent_1 (bash)
    waitpid(0, &status, 0);
    return TRUE;
}

/* Execute commands. If there are non-Builtin commands,
   pass it to nonBuiltin function */
int exeCmds(DynArray_T oCmds, const char *programName){
    assert(oCmds);

    int numCmds = DynArray_getLength(oCmds);
    if(!numCmds) return TRUE;
    
    Cmd_T psCmd = (Cmd_T)DynArray_get(oCmds, 0);
    char *cmdName = psCmd->cmdName;

    /* Check if built-in cmd */
    if(strcmp(cmdName, "setenv\0")==0) {
        if(!exeSetenv(oCmds, programName))
            return FALSE;
        return TRUE;
    }
    if(strcmp(cmdName, "unsetenv\0")==0) {
        if(!exeUnsetenv(oCmds, programName))
            return FALSE;
        return TRUE;
    }
    if(strcmp(cmdName, "cd\0")==0) {
        if(!exeCd(oCmds, programName))
            return FALSE;
        return TRUE;
    }
    if(strcmp(cmdName, "exit\0")==0) {
        if(!exeExit(oCmds, programName))
            return FALSE;
        return TRUE;
    }

    /* If Not built-in cmd */
    nonBuiltin(oCmds, numCmds, programName);
    return TRUE;
}