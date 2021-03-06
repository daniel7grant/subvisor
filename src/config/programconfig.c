#include "programconfig.h"

ProgramConfiguration createdefaultprogramconfig(char *name)
{
	ProgramConfiguration program;
	strcpy(program.command, "");
	strcpy(program.process_name, name);
	program.numprocs = 1;
	program.numprocs_start = 0;
	program.umask = -1;
	program.priority = 999;
	program.autostart = 1;
	program.autorestart = UNEXPECTED;
	program.startsecs = 1;
	program.startretries = 3;
	memset(program.exitcodes, 0, MAX_EXITCODES * sizeof(short));
	program.exitcodes[0] = 1;
	program.stopsignal = 0;
	program.stopwaitsecs = 10;
	program.stopasgroup = 0;
	program.killasgroup = 0;
	program.redirect_stderr = 0;
	strcpy(program.user, "");
	strcpy(program.directory, "");
	strcpy(program.environment, "");

	program.stdout_log = createlogger("");
	program.stderr_log = createlogger("");

	return program;
}

ProgramList *peekprogramlist(ProgramList *programlist)
{
	ProgramList *tip = programlist;
	while (tip->next != NULL)
	{
		tip = tip->next;
	}
	return tip;
}

ProgramList *pushtoprogramlist(ProgramList *programlist, ProgramConfiguration program)
{
	ProgramList *newprogramlist = (ProgramList *)malloc(sizeof(ProgramList));
	newprogramlist->program = program;
	newprogramlist->next = NULL;
	if (programlist == NULL)
	{
		return programlist = newprogramlist;
	}
	ProgramList *tip = peekprogramlist(programlist);
	tip->next = newprogramlist;
	return programlist;
}

int countprogramlist(ProgramList *programlist)
{
	int i = 0;
	ProgramList *tip = programlist;
	while (tip != NULL)
	{
		tip = tip->next;
		++i;
	}
	return i;
}

void freeprogramlist(ProgramList *programlist)
{
	while (programlist != NULL)
	{
		ProgramList *tmp = programlist;
		programlist = programlist->next;
		freelogger(tmp->program.stdout_log);
		freelogger(tmp->program.stderr_log);
		free(tmp);
	}
}
