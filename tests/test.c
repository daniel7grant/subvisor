#include "../unity/src/unity.h"
#include "config/arguments.c"
#include "config/configtypes.c"

void setUp() {}
void tearDown() {}

int main(void)
{
	UNITY_BEGIN();
	test_arguments();
	test_configtypes();
	return UNITY_END();
}