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
	"/etc/subvisord.conf",
	"/etc/supervisord.conf",
	"/etc/subvisor/subvisord.conf",
	"/etc/supervisor/supervisord.conf",
};
int defaultconfigurationcount = sizeof(defaultconfigurationfiles) / sizeof(defaultconfigurationfiles[0]);

const char pathseparator = '/';

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
	getcwd(cwd, MAX_LINE_LENGTH);
	if (filename != NULL)
	{
		joinpath(cwd, filename);
	}
	return cwd;
}

int getcurrentuserid()
{
	return getuid();
}

int getuserid(const char *name)
{
	struct passwd *user = getpwnam(name);
	return user != NULL ? (int)user->pw_uid : -1;
}

int checkaccess(const char *path, int writeable)
{
	return access(path, writeable ? (W_OK | R_OK) : R_OK) == 0;
}

const char *gettempdir()
{
	const char *tmpdirs[] = {
		getenv("TMPDIR"),
		getenv("TEMP"),
		getenv("TMP"),
		"/tmp",
		"/var/tmp",
		"/usr/tmp"};
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

long long int getms()
{
#ifdef _POSIX_MONOTONIC_CLOCK
	struct timespec ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
	{
		return (long long int)(ts.tv_sec * 1000 + ts.tv_nsec / 1e6);
	}
#else
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == 0)
	{
		return (long long int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
	}
#endif
	return 0;
}