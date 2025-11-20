/*
 * Test Common - Lightweight test framework
 * Copyright (c) 2025 OpenPajak Contributors
 */

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>

/* Global test counters */
extern int g_test_total;
extern int g_test_passed;
extern int g_test_failed;

/* Include types for portable printf macros */
#include <pph/pph_types.h>

/* Test macros */
#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "FAIL: %s:%d: Expected %" PPH_PRId64 ", got %" PPH_PRId64 "\n", \
                    __FILE__, __LINE__, \
                    (pph_int64_t)(expected), (pph_int64_t)(actual)); \
            return 1; \
        } \
    } while(0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "FAIL: %s:%d: Assertion failed: %s\n", \
                    __FILE__, __LINE__, #condition); \
            return 1; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "FAIL: %s:%d: Expected non-NULL pointer\n", \
                    __FILE__, __LINE__); \
            return 1; \
        } \
    } while(0)

#define TEST(name) \
    static int test_##name(void)

#define RUN_TEST(name) \
    do { \
        printf("  %-50s ", #name); \
        fflush(stdout); \
        if (test_##name() == 0) { \
            printf("[PASS]\n"); \
            g_test_passed++; \
        } else { \
            printf("[FAIL]\n"); \
            g_test_failed++; \
        } \
        g_test_total++; \
    } while(0)

#define TEST_SUMMARY() \
    do { \
        printf("\n========================================\n"); \
        printf("Total: %d, Passed: %d, Failed: %d\n", \
               g_test_total, g_test_passed, g_test_failed); \
        printf("========================================\n"); \
    } while(0)

#endif /* TEST_COMMON_H */
