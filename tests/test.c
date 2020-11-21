#include "../unity/src/unity.h"
#include "config/arguments.c"

void setUp() {}
void tearDown() {}

int main(void)
{
	UNITY_BEGIN();
	test_arguments();
	return UNITY_END();
}