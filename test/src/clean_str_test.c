#include "testlib.h"
#include <cleaner.h>

static void println_nfmt(const char* buf)
{
	int i;
	for (i=0; buf[i]!=0; ++i) {
		if (buf[i] == '\r') {
			putchar('\\');
			putchar('r');
		} else if (buf[i] == '\n') {
			putchar('\\');
			putchar('n');
		} else {
			putchar(buf[i]);
		}
	}
	putchar('\n');
}

TEST(add_missing_new_line_eof_works)
{
	char buf[MAX_FILE_SIZE] = "\nA";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_trailing_new_line_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\n";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf))
}

TEST(trim_multiple_trailing_new_lines_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\n\n\n";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf))
}

TEST(trim_new_lines_in_file_start_works)
{
	char buf[MAX_FILE_SIZE] = "\nA\nB  C\n";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\nB  C\n", buf));
}

TEST(add_missing_new_line_eof_w_trailing_whitespace_works)
{
	char buf[MAX_FILE_SIZE] = "\nA   ";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_trailing_whitespace_before_trailing_new_lines_eof_works)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_leading_new_lines_lf_works)
{
	char buf[MAX_FILE_SIZE] = "\nA";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_multiple_leading_new_lines_lf_works)
{
	char buf[MAX_FILE_SIZE] = "\n\nA";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST(trim_leading_new_lines_crlf_works)
{
	char buf[MAX_FILE_SIZE] = "\r\nA";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\r\n", buf));
}

TEST(trim_multiple_leading_new_lines_crlf_works)
{
	char buf[MAX_FILE_SIZE] = "\r\n\r\nA";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\r\n", buf));
}

TEST(correct_line_ending_crlf_to_lf_works)
{
	char buf[MAX_FILE_SIZE] = "A\nB\r\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\nB\nc\n", buf));
}

TEST(correct_line_ending_crlf_to_lf_blank_line_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\r\nB\r\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n\nB\nc\n", buf));
}

TEST(correct_line_ending_crlf_to_lf_blank_lines_works)
{
	char buf[MAX_FILE_SIZE] = "A\n\r\n\r\n\n\r\nB\r\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n\nB\nc\n", buf));
}

TEST(correct_line_ending_lf_to_crlf_works)
{
	char buf[MAX_FILE_SIZE] = "A\r\nB\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\r\nB\r\nc\r\n", buf));
}

TEST(correct_line_ending_lf_to_crlf_blank_line_works)
{
	char buf[MAX_FILE_SIZE] = "A\r\n\nB\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\r\n\r\nB\r\nc\r\n", buf));
}

TEST(correct_line_ending_lf_to_crlf_blank_lines_works)
{
	char buf[MAX_FILE_SIZE] = "A\r\n\n\n\r\nB\nc";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\r\n\r\nB\r\nc\r\n", buf));
}

TEST(correct_line_ending_to_lf_works)
{
	char buf[MAX_FILE_SIZE] = "A\r\nB\nc";
	clean_str(buf, strlen(buf), LE_LF);
	ASSERT(!strcmp("A\nB\nc\n", buf));
}

TEST(correct_line_ending_to_crlf_works)
{
	char buf[MAX_FILE_SIZE] = "A\nB\r\nc";
	clean_str(buf, strlen(buf), LE_CRLF);
	ASSERT(!strcmp("A\r\nB\r\nc\r\n", buf));
}

TEST(test_01)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n    ";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}


TEST(test_02)
{
	char buf[MAX_FILE_SIZE] = "A     \n\n    \n";
	clean_str(buf, strlen(buf), LE_UNKNOWN);
	ASSERT(!strcmp("A\n", buf));
}

TEST_MAIN(
	add_missing_new_line_eof_works,
	trim_trailing_new_line_eof_works,
	trim_multiple_trailing_new_lines_eof_works,
	trim_new_lines_in_file_start_works,
	add_missing_new_line_eof_w_trailing_whitespace_works,
	trim_trailing_whitespace_before_trailing_new_lines_eof_works,
	trim_leading_new_lines_lf_works,
	trim_multiple_leading_new_lines_lf_works,
	trim_leading_new_lines_crlf_works,
	trim_multiple_leading_new_lines_crlf_works,
	correct_line_ending_crlf_to_lf_works,
	correct_line_ending_crlf_to_lf_blank_line_works,
	correct_line_ending_crlf_to_lf_blank_lines_works,
	correct_line_ending_lf_to_crlf_works,
	correct_line_ending_lf_to_crlf_blank_line_works,
	correct_line_ending_lf_to_crlf_blank_lines_works,
	correct_line_ending_to_lf_works,
	correct_line_ending_to_crlf_works,
	test_01,
	test_02)
