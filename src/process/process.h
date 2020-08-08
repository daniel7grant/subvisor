#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config/programconfig.h"

typedef enum PROCESS_STATE
{
	STOPPED = 0,
	STARTING = 10,
	RUNNING = 20,
	BACKOFF = 30,
	STOPPING = 40,
	EXITED = 100,
	FATAL = 200,
	UNKNOWN = 1000
} PROCESS_STATE;

typedef struct Process
{
	int pid;
	ProgramConfiguration config;
	PROCESS_STATE state;
	FILE *pipe;
	int retries;
	int stdoutfd;
	int stderrfd;
} Process;

extern Process *processes;
extern int processcount;

extern Process *findprocess(int pid);
extern int shouldrestart(Process *process, int code);

#endif
