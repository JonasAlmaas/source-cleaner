#include "testlib.h"
#include <cleaner.h>

TEST(add_missing_new_line_eof)
{
	char buf[MAX_FILE_SIZE] = "\nA";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_trailing_new_line_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\n";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf))
}

TEST(trim_multiple_trailing_new_lines_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\n\n\n";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf))
}

TEST(trim_new_lines_in_file_start_works)
{
	char buf[MAX_FILE_SIZE] = "\nA\nB  C\n";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\nB  C\n", buf));
}

TEST(add_missing_new_line_eof_w_trailing_whitespace)
{
	char buf[MAX_FILE_SIZE] = "\nA   ";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_trailing_whitespace_before_trailing_new_lines_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf));
}

TEST(test_01)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n    ";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf));
}


TEST(test_02)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n    \n";
	src_clean(buf, strlen(buf));
	ASSERT(!strcmp("A\n", buf));
}

TEST_MAIN(
	add_missing_new_line_eof,
	trim_trailing_new_line_eof_works,
	trim_multiple_trailing_new_lines_eof_works,
	trim_new_lines_in_file_start_works,
	add_missing_new_line_eof_w_trailing_whitespace,
	trim_trailing_whitespace_before_trailing_new_lines_eof_works,
	test_01,
	test_02)
