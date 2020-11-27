#include "configtypes.h"

int toboolean(char *var)
{
	if (strcmp(var, "true") == 0 || strcmp(var, "yes") == 0 || strcmp(var, "on") == 0 || strcmp(var, "1") == 0)
	{
		return 1;
	}
	if (strcmp(var, "false") == 0 || strcmp(var, "no") == 0 || strcmp(var, "off") == 0 || strcmp(var, "0") == 0)
	{
		return 0;
	}
	return -1;
}

int tonumber(char *var, int base)
{
	char *end;
	int value = strtol(var, &end, base);
	if (end == var || *end != '\0' || errno == ERANGE || value < 0)
	{
		return -1;
	}
	return value;
}

long long int tobyte(char *var)
{
	long long int multiplier = 1;
	int varlength = strlen(var);
	if (var[varlength - 1] == 'b' || var[varlength - 1] == 'B')
	{
		switch (var[varlength - 2])
		{
		case 'k':
		case 'K':
			multiplier = 1024;
			var[varlength - 1] = '\0';
			var[varlength - 2] = '\0';
			break;
		case 'm':
		case 'M':
			multiplier = 1024 * 1024;
			var[varlength - 1] = '\0';
			var[varlength - 2] = '\0';
			break;
		case 'g':
		case 'G':
			multiplier = 1024 * 1024 * 1024;
			var[varlength - 1] = '\0';
			var[varlength - 2] = '\0';
			break;
		default:
			return -1;
		}
	}
	int value = tonumber(var, 10);
	return value != -1 ? multiplier * value : -1;
}

LOGLEVEL tologlevel(char *var)
{
	if (strcmp(var, "critical") == 0)
	{
		return CRITICAL;
	}
	if (strcmp(var, "error") == 0)
	{
		return ERROR;
	}
	if (strcmp(var, "warn") == 0)
	{
		return WARN;
	}
	if (strcmp(var, "info") == 0)
	{
		return INFO;
	}
	if (strcmp(var, "debug") == 0)
	{
		return DEBUG;
	}
	if (strcmp(var, "trace") == 0)
	{
		return TRACE;
	}
	if (strcmp(var, "blather") == 0)
	{
		return BLATHER;
	}
	return -1;
}

short *toexitcode(char *var)
{
	short *exitcodes = (short *)malloc(MAX_EXITCODES * sizeof(short));
	memset(exitcodes, 0, MAX_EXITCODES * sizeof(short));
	short code;
	char *lastvalue = var;
	char original = var[0];
	for (int i = 0; original != 0; ++i)
	{
		original = var[i];
		if (var[i] == ',' || var[i] == 0)
		{
			var[i] = 0;
			code = tonumber(lastvalue, 10);
			if (code == -1 || code >= MAX_EXITCODES)
			{
				return NULL;
			}
			exitcodes[code] = 1;
			lastvalue = &var[i] + 1;
			var[i] = original;
		}
	}
	return exitcodes;
}

PROGRAM_AUTORESTART toautorestart(char *var)
{
	int boolval = toboolean(var);
	if (boolval != -1)
	{
		return boolval;
	}
	return strcmp(var, "unexpected") == 0 ? UNEXPECTED : -1;
}

glob_t toglob(char *value)
{
	glob_t globbuf;
	char *lastvalue = value;
	char original = value[0];
	for (int i = 0; original != 0; ++i)
	{
		original = value[i];
		if (value[i] == 32 || value[i] == 0)
		{
			value[i] = 0;
			if (strlen(lastvalue))
			{
				glob(lastvalue, ((lastvalue != value) ? GLOB_APPEND : 0) | GLOB_MARK, NULL, &globbuf);
			}
			lastvalue = &value[i] + 1;
			value[i] = original;
		}
	}
	return globbuf;
}