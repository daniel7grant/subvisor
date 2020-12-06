#define TESTING

#include "../unity/src/unity.h"
#include "config/arguments.c"
#include "config/config.c"
#include "config/configtypes.c"
#include "config/programconfig.c"

void setUp() {}
void tearDown() {}

int main(void)
{
	UNITY_BEGIN();
	test_arguments();
	test_config();
	test_configtypes();
	test_programconfig();
	return UNITY_END();
}
