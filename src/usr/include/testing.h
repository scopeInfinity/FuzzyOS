#pragma once

#include <stdio.h>

#ifdef __cplusplus
namespace {
#endif

// these tests are meant to be executed sequentially.

static int test_task_failed;
static int test_count_success;
static int test_count_failure;

#define __ASSERT(msg, x)                                                       \
    do {                                                                       \
        if (!(x)) {                                                            \
            test_task_failed = 1;                                              \
            std::printf("[%d]: %s failed.\n", __LINE__, (msg));                \
        }                                                                      \
    } while (0);
#define ASSERT_TRUE(x) __ASSERT("ASSERT_TRUE(" #x ")", (x))
#define ASSERT_FALSE(x) __ASSERT("ASSERT_FALSE(" #x ")", (!(x)))
#define ASSERT_EQ(x, y) __ASSERT("ASSERT_EQ(" #x "," #y ")", (x) == (y))
#define ASSERT_NE(x, y) __ASSERT("ASSERT_NE(" #x "," #y ")", (x) != (y))

#define RUN_TEST(test_name)                                                    \
    do {                                                                       \
        test_task_failed = 0;                                                  \
        test_name();                                                           \
        if (test_task_failed) {                                                \
            test_count_failure++;                                              \
            std::puts("Test failed: " #test_name "\n");                        \
        } else {                                                               \
            test_count_success++;                                              \
            std::puts("Test passed: " #test_name "\n");                        \
        }                                                                      \
    } while (0)

#define TEST_INIT()                                                            \
    do {                                                                       \
        test_count_success = 0;                                                \
        test_count_failure = 0;                                                \
    } while (0)
#define TEST_SUMMARY()                                                         \
    do {                                                                       \
        std::printf("[%s] TEST_SUCCESS_COUNT: %d\n", __FILE__,                 \
                    test_count_success);                                       \
        std::printf("[%s] TEST_FAILURE_COUNT: %d\n", __FILE__,                 \
                    test_count_failure);                                       \
    } while (0)

#ifdef __cplusplus
} // namespace end
#endif
