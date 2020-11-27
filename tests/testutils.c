#include "testutils.h"

char *str(const char *input) {
	// fix segfault errors within tests
	char *s = (char *)malloc(sizeof(input));
	strcpy(s, input);
	return s;
}