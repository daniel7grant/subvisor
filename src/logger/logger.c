#include "logger.h"
#include "../utils/platform.h"

Logger createlogger(const char *logfile)
{
	Logger logger;
	strcpy(logger.logfile, logfile);
	logger.logfile_maxbytes = DEFAULT_LOG_MAXBYTES;
	logger.loglevel = 0;
	logger.logfile_backups = 10;
	logger.capture_maxbytes = 0;
	logger.syslog = 0;
	logger.events_enabled = 0;
	logger.logwriter = NULL;
	return logger;
}

int openlogger(Logger *logger)
{
	if (logger->syslog)
	{
		return EXIT_SUCCESS;
	}
	logger->logwriter = fopen(logger->logfile, "w");
	if (logger->logwriter == NULL)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int formatlogger(Logger *logger, int loglevel, char *format, ...)
{
	if (logger->logwriter == NULL)
	{
		return EXIT_FAILURE;
	}
	if (logger->syslog)
	{
		return EXIT_SUCCESS;
	}
	va_list args;
	va_start(args, format);
	vfprintf(logger->logwriter, format, args);
	va_end(args);
	fflush(logger->logwriter);
	return EXIT_SUCCESS;
}

int writelogger(Logger *logger, int loglevel, char *msg)
{
	if (logger->logwriter == NULL)
	{
		return EXIT_FAILURE;
	}
	if (logger->syslog)
	{
		return EXIT_SUCCESS;
	}
	fputs(msg, logger->logwriter);
	fflush(logger->logwriter);
	return EXIT_SUCCESS;
}

void freelogger(Logger logger)
{
	if (logger.logwriter != NULL)
	{
		fclose(logger.logwriter);
	}
}