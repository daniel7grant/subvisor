#include "process.h"

Process *processes = NULL;
int processcount = 0;

Process *findprocess(int pid)
{
	if (processes != NULL)
	{
		for (int i = 0; i < processcount; ++i)
		{
			if (processes[i].pid == pid)
			{
				return &processes[i];
			}
		}
	}
	return NULL;
}

int shouldrestart(Process *process, int code)
{
	return process->config->autorestart == UNEXPECTED
			   ? process->config->exitcodes != code
			   : process->config->autorestart;
}