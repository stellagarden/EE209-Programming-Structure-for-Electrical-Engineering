#ifndef CMD_H
#define CMD_H

enum CmdType {CMD_NONFUNC, CMD_RB, CMD_LB};

struct Cmd{
   enum CmdType cmdType;
   char *cmdName;
   char **argv;
   int arglen;
};

typedef struct Cmd * Cmd_T;

/* Make new Cmd structure consist of arglen, argv, cmdType
   arglen: lenght of argv
   argv: arguments that this command have
   cmdType: indicates that this cmd is normal command or > or < or | */
Cmd_T makeCmd(int arglen, char **argv, enum CmdType cmdType);

/* Free cmd. pvExtra is unused */
void freeCmd(void *pvItem, void *pvExtra);

/* Print cmd. pvExtra is unused */
void printCmd(void *pvItem, void *pvExtra);

#endif