#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config/programconfig.h"

typedef enum ProcessState
{
	STOPPED = 0,
	STARTING = 10,
	RUNNING = 20,
	BACKOFF = 30,
	STOPPING = 40,
	EXITED = 100,
	FATAL = 200,
	UNKNOWN = 1000
} ProcessState;

typedef struct Process
{
	int pid;
	ProgramConfiguration config;
	ProcessState state;
	FILE *pipe;
} Process;

#endif
