#include "platform.h"

const char *defaultconfigurationfiles[] = {
	"../etc/subvisord.conf",
	"../etc/supervisord.conf",
	"../subvisord.conf",
	"../supervisord.conf",
	"./subvisord.conf",
	"./supervisord.conf",
	"./etc/subvisord.conf",
	"./etc/supervisord.conf",
};
int defaultconfigurationcount = sizeof(defaultconfigurationfiles) / sizeof(defaultconfigurationfiles[0]);

const char pathseparator = '\\';

char *joinpath(char *dir, char *filename)
{
	if (filename != NULL)
	{
		int length = strlen(dir);
		if (dir[length - 1] != pathseparator)
		{
			dir[length] = pathseparator;
			dir[length + 1] = '\0';
		}
		strcat(dir, filename);
	}
	return dir;
}

char *getcurrentdirfile(char *filename)
{
	char *cwd = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
	memset(cwd, 0, MAX_LINE_LENGTH);
	_getcwd(cwd, MAX_LINE_LENGTH);
	if (filename != NULL)
	{
		joinpath(cwd, filename);
	}
	return cwd;
}

int getcurrentuserid()
{
	return 0;
}

int getuserid(const char *name)
{
	// TODO: warning, it translates to a noop in Windows
	return 0;
}

int checkaccess(const char *path, int writeable)
{
	return _access(path, writeable ? 6 : 4) == 0;
}

const char *gettempdir()
{
	const char *tmpdirs[] = {
		getenv("TMPDIR"),
		getenv("TEMP"),
		getenv("TMP"),
		"C:\\TEMP",
		"C:\\TMP"
		"\\TEMP",
		"\\TMP"};
	int tmpdirslength = sizeof(tmpdirs) / sizeof(tmpdirs[0]);
	int i = 0;
	while (i < tmpdirslength && (tmpdirs[i] == NULL || !checkaccess(tmpdirs[i], 0)))
	{
		++i;
	};
	if (i < tmpdirslength)
	{
		return tmpdirs[i];
	}
	return getcurrentdirfile(NULL);
}

int prepareparent(Configuration *configuration)
{
	return 0;
}

int openprocess(Process *process)
{
	return 0;
}

int readprocesses(Process processes[], int processcount)
{
	return 0;
}

int closeprocess(Process *process)
{
	return 0;
}

int testsyslog()
{
	// Windows doesn't support syslog
	return 0;
}

int handlesignals()
{
	return 0;
}

int handleprocesssignals()
{
	return 0;
}
