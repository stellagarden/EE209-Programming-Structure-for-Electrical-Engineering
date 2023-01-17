/*--------------------------------------------------------------------*/
/* ish.c                                                              */
/* EE209 Assignment 5                                                 */
/* 20190533 Hyemin Lee                                                */
/* Minimal Unix Shell                                                 */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "processEachLine.h"

enum {MAX_LINE_SIZE = 1024};
enum {DONT_PRINT_COMMAND, PRINT_COMMAND};

const char *programName;
int quitFlag = 0;

/* SIGALRM handler. Set quitFlag to 0 
   if user didn't Type Ctrl-\ in 5 seconds */
static void alarmHandler(int iSig){
    quitFlag = 0;
    fflush(stdout);
}

/* SIGQUIT handler. Make a 5 seconds timer to really exit the program */
static void quitHandler(int iSig){
    if(quitFlag) exit(0);
    quitFlag = 1;
    printf("\nType Ctrl-\\ again within 5 seconds to exit.\n");
    alarm(5);
}

/* Set required signal process */
void setSignal(void){
    sigset_t sSet;
	sigemptyset(&sSet);
    sigaddset(&sSet, SIGINT);
    sigaddset(&sSet, SIGQUIT);
    sigaddset(&sSet, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &sSet, NULL);

    signal(SIGINT, SIG_IGN);
    signal(SIGALRM, alarmHandler);
    signal(SIGQUIT, quitHandler);
}

/* Execute commands in .ishrc */
void goThroughIshrc(FILE *fp){
    assert(fp);
    char pcLine[MAX_LINE_SIZE];
    while(!feof(fp)){
        fgets(pcLine, MAX_LINE_SIZE, fp);
        if(feof(fp)) break;
        processEachLine(PRINT_COMMAND, pcLine, programName);
    }
}

int main(int argc, char *argv[]){
    assert(argv);
    setSignal();

    FILE *fp;
    const char *homedir;
    char ishrcPath[MAX_LINE_SIZE];
    char pcLine[MAX_LINE_SIZE];
    printf("%% ");
    programName = argv[0];

    // Check .ishrc and exectue
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    strcpy(ishrcPath, homedir);
    chdir(ishrcPath);
    if((fp = fopen("./.ishrc", "r"))) goThroughIshrc(fp);

    // Execute stdin commands
    for(;;){
        fgets(pcLine, MAX_LINE_SIZE, stdin);
        processEachLine(DONT_PRINT_COMMAND, pcLine, programName);
    }   

    return 0;
}
