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

int testsyslog()
{
	return 1;
}

int prepareparent(Configuration *configuration)
{
	if (!configuration->nodaemon)
	{
		int forkedid = fork();
		if (forkedid < 0)
		{
			return EXIT_FAILURE;
		}
		if (forkedid > 0)
		{
			// terminate parent
			exit(EXIT_SUCCESS);
		}
		// obtain new session
		setsid();
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		if (strlen(configuration->directory) > 0)
		{
			chdir(configuration->directory);
		}
	}

	FILE *pidfile = fopen(configuration->pidfile, "w");
	if (pidfile == NULL)
	{
		// usage("could not write pidfile %s", configuration->pidfile);
		return EXIT_FAILURE;
	}
	fprintf(pidfile, "%d", getpid());
	fclose(pidfile);

	int currentuid = getcurrentuserid();

	struct rlimit rlfds;
	getrlimit(RLIMIT_NOFILE, &rlfds);
	rlfds.rlim_cur = configuration->minfds;
	if (currentuid == 0)
	{
		rlfds.rlim_max = configuration->minfds;
	}
	if (setrlimit(RLIMIT_NOFILE, &rlfds))
	{
		return EXIT_FAILURE;
	}

	// TODO: setting a process limit makes forks fail
	// struct rlimit rlproc;
	// getrlimit(RLIMIT_NPROC, &rlproc);
	// rlproc.rlim_cur = configuration->minprocs;
	// if (currentuid == 0)
	// {
	// 	rlproc.rlim_max = configuration->minprocs;
	// }
	// if (setrlimit(RLIMIT_NPROC, &rlproc))
	// {
	// 	return EXIT_FAILURE;
	// }

	umask(configuration->umask);

	if (strlen(configuration->user) > 0)
	{
		int uid = getuserid(configuration->user);
		if (uid < 0)
		{
			return EXIT_FAILURE;
		}
		if (uid > 0 && setuid(uid))
		{
			return EXIT_FAILURE;
		}
	}

	// TODO: add environment variables

	return EXIT_SUCCESS;
}

int openprocess(Process *process)
{
	int stdoutfd[2];
	int stderrfd[2];

	int weerror;
	wordexp_t arguments;
	if ((weerror = wordexp(process->config->command, &arguments, WRDE_NOCMD)))
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
		process->state = STARTING;
		process->starttime = time(NULL);
		close(stdoutfd[1]);
		close(stderrfd[1]);
		process->stdoutfd = stdoutfd[0];
		process->stderrfd = stderrfd[0];
		wordfree(&arguments);
		return EXIT_SUCCESS;
	}
	else
	{
		// TODO: open separate processgroup for process? (setpgid)

		close(stdoutfd[0]);
		close(stderrfd[0]);
		if (dup2(stdoutfd[1], STDOUT_FILENO) != STDOUT_FILENO || dup2(stderrfd[1], STDERR_FILENO) != STDERR_FILENO)
		{
			return EXIT_FAILURE;
		}
		close(stdoutfd[1]);
		close(stderrfd[1]);

		umask(process->config->umask);
		if (strlen(process->config->directory) > 0)
		{
			chdir(process->config->directory);
		}
		if (strlen(process->config->user) > 0)
		{
			int uid = getuserid(process->config->user);
			if (uid < 0)
			{
				return EXIT_FAILURE;
			}
			if (uid > 0 && setuid(uid))
			{
				return EXIT_FAILURE;
			}
		}

		execvp(arguments.we_wordv[0], arguments.we_wordv);
		wordfree(&arguments);
		return EXIT_FAILURE;
	}
}

int readprocesses(Process processes[], int processcount)
{
	struct pollfd fds[processcount * 2];
	for (int i = 0; i < processcount; ++i)
	{
		if (processes[i].state == STARTING || processes[i].state == RUNNING || processes[i].state == STOPPING)
		{
			fds[2 * i].fd = processes[i].stdoutfd;
			fds[2 * i].events = POLLIN;
			fds[2 * i].revents = 0;
			fds[2 * i + 1].fd = processes[i].stderrfd;
			fds[2 * i + 1].events = POLLIN;
			fds[2 * i + 1].revents = 0;
		}
		else
		{
			fds[2 * i].fd = -1;
			fds[2 * i].events = POLLIN;
			fds[2 * i].revents = 0;
			fds[2 * i + 1].fd = -1;
			fds[2 * i + 1].events = POLLIN;
			fds[2 * i + 1].revents = 0;
		}
	}

	int pollret = poll(fds, processcount * 2, 1000);
	if (pollret < 0)
	{
		return EXIT_FAILURE;
	}
	if (pollret > 0)
	{
		for (int i = 0; i < processcount; ++i)
		{
			if (fds[2 * i].revents & POLLIN)
			{
				char *buffer = (char *)malloc(4096 * sizeof(char));
				memset(buffer, 0, 4096);
				int count = read(fds[2 * i].fd, buffer, 4096);
				if (count == 0)
				{
					continue;
				}
				if (count < 0)
				{
					return EXIT_FAILURE;
				}
				writelogger(&processes[i].config->stdout_log, 0, buffer);
				free(buffer);
			}

			if (fds[2 * i + 1].revents & POLLIN)
			{
				char *buffer = (char *)malloc(4096 * sizeof(char));
				memset(buffer, 0, 4096);
				int count = read(fds[2 * i + 1].fd, buffer, 4096);
				if (count == 0)
				{
					continue;
				}
				if (count < 0)
				{
					return EXIT_FAILURE;
				}
				writelogger(&processes[i].config->stderr_log, 0, buffer);
				free(buffer);
			}

			// TODO: handle hangups?
		}
	}

	return EXIT_SUCCESS;
}

int closeprocess(Process *process)
{
	int status = EXIT_SUCCESS;
	waitpid(-1, &status, 0);
	return status;
}

void handler(int sig)
{
	switch (sig)
	{
	case SIGTERM:
	case SIGINT:
	case SIGQUIT:
		// TODO: kill processes
		exit(999);
		break;
	case SIGHUP:
		// TODO: restart all processes
	case SIGUSR2:
		// TODO: reopen child logs
		break;
	default:
		break;
	}
}

void childhandler(int sig)
{
	int pid, status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		Process *process = findprocess(pid);
		if (WIFEXITED(status))
		{
			if (shouldrestart(process, WEXITSTATUS(status)) && ++process->retries <= process->config->startretries)
			{
				process->state = BACKOFF;
			}
			else
			{
				if (process->state == RUNNING)
				{
					process->state = EXITED;
				}
				else
				{
					process->state = FATAL;
				}
			}
		}
		else if (WIFSIGNALED(status))
		{
			// TODO: autorestart this?
			if (process->state == STARTING || process->state == RUNNING || process->state == FATAL)
			{
				process->state = STOPPING;
			}
		}
	}
}

int handlesignals()
{
	// TODO: set signal mask with setprocmask

	struct sigaction sa;
	sa.sa_handler = &handler;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	int signals[] = {SIGTERM, SIGINT, SIGQUIT, SIGHUP, SIGUSR2};
	int signalcount = sizeof(signals) / sizeof(signals[0]);
	for (int i = 0; i < signalcount; ++i)
	{
		if (sigaction(signals[i], &sa, 0) == -1)
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int handleprocesssignals()
{
	struct sigaction sa;
	sa.sa_handler = &childhandler;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, 0) == -1)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
