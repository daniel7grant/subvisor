#include "../../unity/src/unity.h"
#include "../../src/config/configtypes.h"

void test_toboolean_handles_truthy()
{
	TEST_ASSERT_EQUAL(1, toboolean("true"));
	TEST_ASSERT_EQUAL(1, toboolean("yes"));
	TEST_ASSERT_EQUAL(1, toboolean("on"));
	TEST_ASSERT_EQUAL(1, toboolean("1"));
}

void test_toboolean_handles_falsey()
{
	TEST_ASSERT_EQUAL(0, toboolean("false"));
	TEST_ASSERT_EQUAL(0, toboolean("no"));
	TEST_ASSERT_EQUAL(0, toboolean("off"));
	TEST_ASSERT_EQUAL(0, toboolean("0"));
}

void test_toboolean_fails_on_invalid()
{
	TEST_ASSERT_EQUAL(-1, toboolean("none"));
	TEST_ASSERT_EQUAL(-1, toboolean("iguana"));
	TEST_ASSERT_EQUAL(-1, toboolean("-1"));
}

void test_tonumber_handles_decimals()
{
	TEST_ASSERT_EQUAL(123, tonumber("123", 10));
	TEST_ASSERT_EQUAL(123, tonumber("123", 10));
}

void test_tonumber_handles_octals()
{
	TEST_ASSERT_EQUAL(0, tonumber("000", 8));
	TEST_ASSERT_EQUAL(18, tonumber("022", 8));
	TEST_ASSERT_EQUAL(511, tonumber("777", 8));
}

void test_tonumber_fails_on_invalid_decimal_or_negative()
{
	TEST_ASSERT_EQUAL(-1, tonumber("zero", 10));
	TEST_ASSERT_EQUAL(-1, tonumber("123.4", 10));
	TEST_ASSERT_EQUAL(-1, tonumber("-123", 10));
}

void test_tobyte_handles_numbers()
{
	TEST_ASSERT_EQUAL(1024, tobyte(str("1024")));
	TEST_ASSERT_EQUAL(100, tobyte(str("100")));
}

void test_tobyte_handles_si_byte_numbers()
{
	TEST_ASSERT_EQUAL(1024 * 100, tobyte(str("100kb")));
	TEST_ASSERT_EQUAL(1024 * 100, tobyte(str("100kB")));
	TEST_ASSERT_EQUAL(1024 * 100, tobyte(str("100Kb")));
	TEST_ASSERT_EQUAL(1024 * 100, tobyte(str("100KB")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 100, tobyte(str("100mb")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 100, tobyte(str("100mB")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 100, tobyte(str("100Mb")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 100, tobyte(str("100MB")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 1024 * (long int)100, tobyte(str("100gb")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 1024 * (long int)100, tobyte(str("100gB")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 1024 * (long int)100, tobyte(str("100Gb")));
	TEST_ASSERT_EQUAL(1024 * 1024 * 1024 * (long int)100, tobyte(str("100GB")));
}

void test_tobyte_fails_on_decimals_bad_si_and_invalids()
{
	TEST_ASSERT_EQUAL(-1, tobyte(str("0.1GB")));
	TEST_ASSERT_EQUAL(-1, tobyte(str("100T")));
	TEST_ASSERT_EQUAL(-1, tobyte(str("100TB")));
	TEST_ASSERT_EQUAL(-1, tobyte(str("oneMB")));
}

void test_tologlevel_handles_correct_logleves()
{
	TEST_ASSERT_EQUAL(CRITICAL, tologlevel("critical"));
	TEST_ASSERT_EQUAL(ERROR, tologlevel("error"));
	TEST_ASSERT_EQUAL(WARN, tologlevel("warn"));
	TEST_ASSERT_EQUAL(INFO, tologlevel("info"));
	TEST_ASSERT_EQUAL(DEBUG, tologlevel("debug"));
	TEST_ASSERT_EQUAL(TRACE, tologlevel("trace"));
	TEST_ASSERT_EQUAL(BLATHER, tologlevel("blather"));
}

void test_tologlevel_fails_on_invalid_loglevels()
{
	TEST_ASSERT_EQUAL((LOGLEVEL)-1, tologlevel("important"));
	TEST_ASSERT_EQUAL((LOGLEVEL)-1, tologlevel("help me"));
}

void test_toexitcode_handles_one_exitcode()
{
	short *exitcodes = (short *)malloc(MAX_EXITCODES * sizeof(short));
	memset(exitcodes, 0, MAX_EXITCODES * sizeof(short));

	exitcodes = toexitcode(str("129"));
	TEST_ASSERT_NOT_NULL(exitcodes);
	for (int i = 0; i < MAX_EXITCODES; i++)
	{
		TEST_ASSERT_EQUAL(i == 129, exitcodes[i]);
	}
}

void test_toexitcode_handles_list_of_exitcodes()
{
	short *exitcodes = (short *)malloc(MAX_EXITCODES * sizeof(short));
	memset(exitcodes, 0, MAX_EXITCODES * sizeof(short));

	exitcodes = toexitcode(str("0,2,129"));
	TEST_ASSERT_NOT_NULL(exitcodes);
	for (int i = 0; i < MAX_EXITCODES; i++)
	{
		TEST_ASSERT_EQUAL(i == 0 || i == 2 || i == 129, exitcodes[i]);
	}
}

void test_toexitcode_fails_on_larger_than_max_exitcodes()
{
	short *exitcodes = (short *)malloc(MAX_EXITCODES * sizeof(short));
	memset(exitcodes, 0, MAX_EXITCODES * sizeof(short));
	exitcodes = toexitcode(str("0,299"));

	TEST_ASSERT_NULL(exitcodes);
}

void test_toexitcode_fails_on_invalid_exitcodes()
{
	short *exitcodes = (short *)malloc(MAX_EXITCODES * sizeof(short));
	memset(exitcodes, 0, MAX_EXITCODES * sizeof(short));
	exitcodes = toexitcode(str("success,failure"));

	TEST_ASSERT_NULL(exitcodes);
}

void test_toautorestart_handles_truthy()
{
	TEST_ASSERT_EQUAL(TRUE, toautorestart("true"));
	TEST_ASSERT_EQUAL(TRUE, toautorestart("yes"));
	TEST_ASSERT_EQUAL(TRUE, toautorestart("on"));
	TEST_ASSERT_EQUAL(TRUE, toautorestart("1"));
}

void test_toautorestart_handles_falsey()
{
	TEST_ASSERT_EQUAL(FALSE, toautorestart("false"));
	TEST_ASSERT_EQUAL(FALSE, toautorestart("no"));
	TEST_ASSERT_EQUAL(FALSE, toautorestart("off"));
	TEST_ASSERT_EQUAL(FALSE, toautorestart("0"));
}

void test_toautorestart_handles_unexpected()
{
	TEST_ASSERT_EQUAL(UNEXPECTED, toautorestart("unexpected"));
}

void test_toautorestart_fails_on_invalid()
{
	TEST_ASSERT_EQUAL((PROGRAM_AUTORESTART)-1, toautorestart("restart"));
	TEST_ASSERT_EQUAL((PROGRAM_AUTORESTART)-1, toautorestart("iguana"));
	TEST_ASSERT_EQUAL((PROGRAM_AUTORESTART)-1, toautorestart("2"));
}

void test_toglob_handles_one_glob()
{
	glob_t glob = toglob(str("src/*.c"));
	TEST_ASSERT_EQUAL(1, glob.gl_pathc);
	TEST_ASSERT_EQUAL_STRING("src/subvisord.c", glob.gl_pathv[0]);
	globfree(&glob);
}

void test_toglob_handles_a_list_of_globs()
{
	glob_t glob = toglob(str("src/*.c tests/test*.c"));
	TEST_ASSERT_EQUAL(3, glob.gl_pathc);
	TEST_ASSERT_EQUAL_STRING("src/subvisord.c", glob.gl_pathv[0]);
	TEST_ASSERT_EQUAL_STRING("tests/test.c", glob.gl_pathv[1]);
	TEST_ASSERT_EQUAL_STRING("tests/testutils.c", glob.gl_pathv[2]);
	globfree(&glob);
}

void test_configtypes()
{
	printf("\n%s\n", "src/config/configtypes");
	RUN_TEST(test_toboolean_handles_truthy);
	RUN_TEST(test_toboolean_handles_falsey);
	RUN_TEST(test_toboolean_fails_on_invalid);
	RUN_TEST(test_tonumber_handles_decimals);
	RUN_TEST(test_tonumber_handles_octals);
	RUN_TEST(test_tonumber_fails_on_invalid_decimal_or_negative);
	RUN_TEST(test_tobyte_handles_numbers);
	RUN_TEST(test_tobyte_handles_si_byte_numbers);
	RUN_TEST(test_tobyte_fails_on_decimals_bad_si_and_invalids);
	RUN_TEST(test_tologlevel_handles_correct_logleves);
	RUN_TEST(test_tologlevel_fails_on_invalid_loglevels);
	RUN_TEST(test_toexitcode_handles_one_exitcode);
	RUN_TEST(test_toexitcode_handles_list_of_exitcodes);
	RUN_TEST(test_toexitcode_fails_on_larger_than_max_exitcodes);
	RUN_TEST(test_toexitcode_fails_on_invalid_exitcodes);
	RUN_TEST(test_toautorestart_handles_truthy);
	RUN_TEST(test_toautorestart_handles_falsey);
	RUN_TEST(test_toautorestart_handles_unexpected);
	RUN_TEST(test_toautorestart_fails_on_invalid);
	RUN_TEST(test_toglob_handles_one_glob);
	RUN_TEST(test_toglob_handles_a_list_of_globs);
}