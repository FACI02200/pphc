/*
 * Test: Money arithmetic
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <pph/pph_calculator.h>
#include "test_common.h"

int g_test_total = 0;
int g_test_passed = 0;
int g_test_failed = 0;

TEST(money_addition) {
    pph_money_t a = PPH_MONEY(100, 5000);  /* 100.5000 */
    pph_money_t b = PPH_MONEY(50, 2500);   /* 50.2500 */
    pph_money_t result = pph_money_add(a, b);

    ASSERT_EQ(1507500, result.value);  /* 150.7500 */
    return 0;
}

TEST(money_subtraction) {
    pph_money_t a = PPH_MONEY(100, 0);
    pph_money_t b = PPH_MONEY(30, 0);
    pph_money_t result = pph_money_sub(a, b);

    ASSERT_EQ(700000, result.value);  /* 70.0000 */
    return 0;
}

TEST(money_percentage) {
    pph_money_t amount = PPH_RUPIAH(1000);
    pph_money_t result = pph_money_percent(amount, 5, 100);  /* 5% */

    ASSERT_EQ(500000, result.value);  /* 50.0000 */
    return 0;
}

TEST(money_round_down_thousand) {
    pph_money_t value = PPH_RUPIAH(12345);
    pph_money_t result = pph_money_round_down_thousand(value);

    ASSERT_EQ(120000000, result.value);  /* 12000 Rupiah */
    return 0;
}

TEST(money_min) {
    pph_money_t a = PPH_RUPIAH(100);
    pph_money_t b = PPH_RUPIAH(50);
    pph_money_t result = pph_money_min(a, b);

    ASSERT_EQ(b.value, result.value);
    return 0;
}

TEST(money_max) {
    pph_money_t a = PPH_RUPIAH(100);
    pph_money_t b = PPH_RUPIAH(50);
    pph_money_t result = pph_money_max(a, b);

    ASSERT_EQ(a.value, result.value);
    return 0;
}

int main(void) {
    printf("========================================\n");
    printf("  Money Arithmetic Tests\n");
    printf("========================================\n\n");

    RUN_TEST(money_addition);
    RUN_TEST(money_subtraction);
    RUN_TEST(money_percentage);
    RUN_TEST(money_round_down_thousand);
    RUN_TEST(money_min);
    RUN_TEST(money_max);

    TEST_SUMMARY();

    return g_test_failed > 0 ? 1 : 0;
}
