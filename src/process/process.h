#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include <poll.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wordexp.h>

#include "../utils/platform.h"
#include "../config/config.h"

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
	ProgramConfiguration *config;
	PROCESS_STATE state;
	long long int starttime;
	int retries;
	int stdoutfd;
	int stderrfd;
} Process;

extern Process *processes;
extern int processcount;

extern Process *findprocess(int pid);
extern void prioritizeprocesses();
extern int shouldrestart(Process *process, int code);
extern int hasstarted(Process *process);

extern int prepareparent(Configuration *configuration);
extern int openprocess(Process *process);
extern int readprocesses(Process processes[], int processcount);
extern int closeprocess(Process *process);

extern int handlesignals();
extern int handleprocesssignals();

#endif
