#ifndef _PROGRAMCONFIG_H_INCLUDED_
#define _PROGRAMCONFIG_H_INCLUDED_

#include "../logger/logger.h"
#include "../utils/utils.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum PROGRAM_AUTORESTART
{
	FALSE = 0,
	TRUE = 1,
	UNEXPECTED = 2
} PROGRAM_AUTORESTART;

typedef struct ProgramConfiguration
{
	char command[MAX_LINE_LENGTH];
	Logger stdout_log;
	Logger stderr_log;
	char process_name[MAX_LINE_LENGTH];
	int numprocs;
	int numprocs_start;
	char directory[MAX_LINE_LENGTH];
	int umask;
	int priority;
	int autostart;
	PROGRAM_AUTORESTART autorestart;
	int startsecs;
	int startretries;
	short exitcodes[MAX_EXITCODES];
	int stopsignal;
	int stopwaitsecs;
	int stopasgroup;
	int killasgroup;
	int redirect_stderr;
	char user[MAX_USERNAME_LENGTH];
	char environment[MAX_LINE_LENGTH];
} ProgramConfiguration;

typedef struct ProgramList
{
	ProgramConfiguration program;
	struct ProgramList *next;
} ProgramList;

extern ProgramConfiguration createdefaultprogramconfig(char *name);

extern ProgramList *pushtoprogramlist(ProgramList *programlist, ProgramConfiguration config);
extern ProgramList *peekprogramlist(ProgramList *programlist);
extern int countprogramlist(ProgramList *programlist);
extern void freeprogramlist(ProgramList *programlist);

#endif