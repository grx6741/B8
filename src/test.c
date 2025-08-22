#include "lexer.h"
#include "parser.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "TEST"
#include "util.h"

typedef struct {
    enum { kTestPassed, kTestFailed } status;
    char* message;
} test_result_t;

#define TEST_FAILED(msg) (test_result_t) { .status = kTestFailed, .message = (msg) }
#define TEST_PASSED()	 (test_result_t) { .status = kTestPassed }

#define FAIL_IF_NOT(condition, ...) \
    if (!(condition)) { \
	int msg_size = snprintf(NULL, 0, __VA_ARGS__);\
	char* msg_buffer = (char*)malloc(msg_size + 1);\
	snprintf(msg_buffer, msg_size, __VA_ARGS__);\
	msg_buffer[msg_size] = '\0';\
	return TEST_FAILED(msg_buffer); \
    }

typedef test_result_t (*test_function_t)(void);

typedef struct {
    test_function_t fn;
    const char* name;
} test_t;

#define MAX_TESTS 255
static test_t tests[MAX_TESTS];
static size_t tests_count;

void register_test(const char* name, test_function_t fn) {
    tests[tests_count++] = (test_t) {
	.fn = fn,
	.name = name
    };
}

#define TEST(name)			    \
    test_result_t test_##name(void);	    \
    __attribute__((constructor))	    \
    void register_test_##name(void) {	    \
	register_test(#name, test_##name);   \
    }					    \
    test_result_t test_##name(void)

#include "tests.def"

int main() {
    if (tests_count == 0) return 0;

    size_t failed_test_count = 0;
    LOG_INFO("Running %d Tests\n", tests_count);

    for (int i = 0; i < tests_count; i++) {
	test_t test = tests[i];
	LOG_INFO("|:: Running Test %s -> ", test.name);
	test_result_t res = test.fn();
	switch (res.status) {
	    case kTestPassed:
		LOG("Passed ::|\n");
		break;
	    case kTestFailed:
		LOG("Failed ::|\n");
		LOG_ERROR("\tReason : %s\n", res.message);
		free(res.message);
		failed_test_count += 1;
		break;
	    default:
		LOG("Invalid Return ::|\n");
		break;
	}
    }

    if (failed_test_count)
	LOG_ERROR("%d/%d Tests Failed\n", failed_test_count, tests_count);
}
