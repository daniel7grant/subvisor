#include "utils.h"

char *arg0 = "";

void usage(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	char *error = malloc(MAX_LINE_LENGTH * sizeof(char));
	memset(error, 0, MAX_LINE_LENGTH);
	vsprintf(error, format, args);
	va_end(args);
	fprintf(stderr, "Error: %s\nFor help, use %s -h\n", error, arg0);
	free(error);
}
