#ifndef TEST_LIB_H_INCLUDED
#define TEST_LIB_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define FILENAME (strrchr("/" __FILE__, '/') + 1)

#define INTERNAL_ASSERT(expr, assert) \
	if (!(expr)) { \
		printf("%s:%d - expected: %s\n", FILENAME, __LINE__, #expr); \
		*test_failed = 1; \
		if (assert) return; \
	}

#define EXPECT(expr) INTERNAL_ASSERT(expr, 0)
#define ASSERT(expr) INTERNAL_ASSERT(expr, 1)

struct test_info {
	void (*fn)(int *test_failed);
	const char *name;
};

#define TEST(name) \
	static void name##_fn(int *test_failed); \
	static const struct test_info name = {name##_fn, #name}; \
	static void name##_fn(int *test_failed)

#define TEST_MAIN(...) \
	int main(void) \
	{ \
		const struct test_info tests[] = {__VA_ARGS__}; \
		int i, test_failed, failed_tests=0; \
		for (i=0; i < sizeof tests / sizeof(struct test_info); ++i) { \
			test_failed = 0; \
			tests[i].fn(&test_failed); \
			if (test_failed) ++failed_tests; \
			printf("%s: %s\n", test_failed ? "FAILED" : "PASSED", tests[i].name); \
		} \
		return failed_tests ? 1 : 0; \
	}

#endif /* TEST_LIB_H_INCLUDED */
