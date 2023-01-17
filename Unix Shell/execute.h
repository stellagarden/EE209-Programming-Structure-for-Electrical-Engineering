#ifndef EXECUTE_H
#define EXECUTE_H

#include "dynarray.h"
#include "cmd.h"

/* Execute non-Builtin commands */
int nonBuiltin(DynArray_T oCmds, int numCmds, const char *programName);

/* Execute commands. If there are non-Builtin commands,
   pass it to nonBuiltin function */
int exeCmds(DynArray_T oCmds, const char *programName);

#endif