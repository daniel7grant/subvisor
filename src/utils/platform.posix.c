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

int checkuser(const char *name)
{
	// TODO: figure out how to fix with statically linked
	struct passwd *user = getpwnam(name);
	return user != NULL;
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

int getprocessid()
{
	return getpid();
}

int openprocess(Process *process)
{
	int stdoutfd[2];
	int stderrfd[2];

	int weerror;
	wordexp_t arguments;
	if ((weerror = wordexp(process->config.command, &arguments, WRDE_NOCMD)))
	{
		if (weerror == WRDE_NOSPACE)
		{
			wordfree(&arguments);
		}
		return EXIT_FAILURE;
	}

	if (pipe(stdoutfd) < 0 || pipe(stderrfd) < 0)
	{
		return EXIT_FAILURE;
	}

	if ((process->pid = fork()) < 0)
	{
		return EXIT_FAILURE;
	}
	else if (process->pid > 0)
	{
		close(stdoutfd[1]);
		process->fd = stdoutfd[0];
		wordfree(&arguments);
		return EXIT_SUCCESS;
	}
	else
	{
		close(stdoutfd[0]);
		if (dup2(stdoutfd[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
			return EXIT_FAILURE;
		}
		close(stdoutfd[1]);
		return execvp(arguments.we_wordv[0], arguments.we_wordv);
	}
}

int readprocesses(Process processes[], int processcount)
{
	fd_set active_fd_set;
	struct timeval timeout;
	char buffer[4096];

	while (1)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		FD_ZERO(&active_fd_set);
		for (int i = 0; i < processcount; i++)
		{
			FD_SET(processes[i].fd, &active_fd_set);
		}

		if (select(FD_SETSIZE, &active_fd_set, NULL, NULL, &timeout) < 0)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; ++i)
		{
			if (FD_ISSET(i, &active_fd_set))
			{
				ssize_t count = read(i, buffer, 1024);
				if (count == 0)
				{
					return EXIT_SUCCESS;
				}
				if (count < 0)
				{
					return EXIT_FAILURE;
				}
				write(STDOUT_FILENO, buffer, count);
			}
		}
	}
}

int closeprocess(Process *process)
{
	int status = EXIT_SUCCESS;
	waitpid(-1, &status, 0);
	return status;
}

int testsyslog()
{
	return 1;
}