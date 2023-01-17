#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#include "dynarray.h"
#include "cmd.h"
#include "builtin.h"

enum {MAX_LINE_SIZE = 1024};
enum {FALSE, TRUE};

/* Execute built-in function setenv */
int exeSetenv(DynArray_T oCmds, const char *programName){
    assert(oCmds);

    if(DynArray_getLength(oCmds)>1){
        fprintf(stderr, "%s: setenv takes one or two parameters\n", programName);
        return FALSE;
    }

    Cmd_T psCmd = (Cmd_T)DynArray_get(oCmds, 0);
    int arglen = psCmd->arglen;
    char **argv = psCmd->argv;
    if((arglen>3) || (arglen<2)){
        fprintf(stderr, "%s: setenv takes one or two parameters\n", programName);
        return FALSE;
    }

    char *src = (argv[2]==NULL) ? "" : argv[2];
    if(setenv(argv[1], src, 1)==-1){
        perror(programName);
        // fprintf(stderr, "%s: setenv failed\n", programName);
        return FALSE;
    }
    // printf("%s - %s\n", argv[1], src);
    return TRUE;
}

/* Execute built-in function unsetenv */
int exeUnsetenv(DynArray_T oCmds, const char *programName){
    assert(oCmds);

    Cmd_T psCmd = (Cmd_T)DynArray_get(oCmds, 0);
    int arglen = psCmd->arglen;
    char **argv = psCmd->argv;
    if(arglen!=2){
        fprintf(stderr, "%s: unsetenv takes one parameter\n", programName);
        return FALSE;
    }
    if(unsetenv(argv[1])==-1){
        perror(programName);
        // fprintf(stderr, "%s: unsetenv failed\n", programName);
        return FALSE;
    }
    // printf("%s - Completely deleted\n", argv[1]);
    return TRUE;
}

/* Execute built-in function cd */
int exeCd(DynArray_T oCmds, const char *programName){
    assert(oCmds);

    Cmd_T psCmd = (Cmd_T)DynArray_get(oCmds, 0);
    int arglen = psCmd->arglen;
    char **argv = psCmd->argv;
    const char *homedir;
    char path[MAX_LINE_SIZE];

    if(arglen>2){
        fprintf(stderr, "%s: cd takes one parameter\n", programName);
        return FALSE;
    }
    if(argv[1]==NULL){
        if ((homedir = getenv("HOME")) == NULL)
            homedir = getpwuid(getuid())->pw_dir;
        strcpy(path, homedir);
    }
    else strcpy(path, argv[1]);

    if(chdir(path)==-1){
        perror(programName);
        return FALSE;
    }
    // printf("Maybe moved to %s :)\n", path);
    return TRUE;
}

/* Execute built-in function exit */
int exeExit(DynArray_T oCmds, const char *programName){
    assert(oCmds);

    Cmd_T psCmd = (Cmd_T)DynArray_get(oCmds, 0);
    if(psCmd->arglen==1) exit(0);
    fprintf(stderr, "%s: exit does not take any parameters\n", programName);
    return FALSE;
}
