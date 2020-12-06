#include "testutils.h"

char *str(const char *input)
{
	// fix segfault errors within tests
	return strdup(input);
}