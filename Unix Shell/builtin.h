#ifndef BUILTIN_H
#define BUILTIN_H

#include "dynarray.h"

/* Execute built-in function setenv */
int exeSetenv(DynArray_T oCmds, const char *programName);

/* Execute built-in function unsetenv */
int exeUnsetenv(DynArray_T oCmds, const char *programName);

/* Execute built-in function cd */
int exeCd(DynArray_T oCmds, const char *programName);

/* Execute built-in function exit */
int exeExit(DynArray_T oCmds, const char *programName);

#endif