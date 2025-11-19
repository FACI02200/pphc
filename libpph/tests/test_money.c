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

TEST(parse_id_basic_integer) {
    pph_money_t result = pph_money_from_string_id("1000");
    ASSERT_EQ(10000000, result.value);  /* 1000.0000 */
    return 0;
}

TEST(parse_id_with_decimal) {
    pph_money_t result = pph_money_from_string_id("100,50");
    ASSERT_EQ(1005000, result.value);  /* 100.5000 */
    return 0;
}

TEST(parse_id_with_thousands) {
    pph_money_t result = pph_money_from_string_id("1.000.000");
    ASSERT_EQ(10000000000, result.value);  /* 1000000.0000 */
    return 0;
}

TEST(parse_id_with_thousands_and_decimal) {
    pph_money_t result = pph_money_from_string_id("1.234.567,89");
    ASSERT_EQ(12345678900, result.value);  /* 1234567.8900 */
    return 0;
}

TEST(parse_id_four_decimals) {
    pph_money_t result = pph_money_from_string_id("100,1234");
    ASSERT_EQ(1001234, result.value);  /* 100.1234 */
    return 0;
}

TEST(parse_id_less_than_four_decimals) {
    pph_money_t result = pph_money_from_string_id("100,5");
    ASSERT_EQ(1005000, result.value);  /* 100.5000 (padded) */
    return 0;
}

TEST(parse_id_negative) {
    pph_money_t result = pph_money_from_string_id("-1.234,56");
    ASSERT_EQ(-12345600, result.value);  /* -1234.5600 */
    return 0;
}

TEST(parse_id_with_whitespace) {
    pph_money_t result = pph_money_from_string_id("  1000,50  ");
    ASSERT_EQ(10005000, result.value);  /* 1000.5000 */
    return 0;
}

TEST(parse_id_zero) {
    pph_money_t result = pph_money_from_string_id("0");
    ASSERT_EQ(0, result.value);
    return 0;
}

TEST(parse_id_zero_with_decimal) {
    pph_money_t result = pph_money_from_string_id("0,0");
    ASSERT_EQ(0, result.value);
    return 0;
}

TEST(parse_id_null) {
    pph_money_t result = pph_money_from_string_id(NULL);
    ASSERT_EQ(0, result.value);
    return 0;
}

TEST(parse_id_empty) {
    pph_money_t result = pph_money_from_string_id("");
    ASSERT_EQ(0, result.value);
    return 0;
}

TEST(parse_id_invalid_chars) {
    pph_money_t result = pph_money_from_string_id("abc123");
    ASSERT_EQ(0, result.value);  /* Returns zero on error */
    return 0;
}

TEST(parse_id_large_number) {
    pph_money_t result = pph_money_from_string_id("250.000.000,00");
    ASSERT_EQ(2500000000000, result.value);  /* 250000000.0000 */
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

    printf("\n");
    printf("  String Parser Tests (Indonesian Format)\n");
    printf("========================================\n\n");

    RUN_TEST(parse_id_basic_integer);
    RUN_TEST(parse_id_with_decimal);
    RUN_TEST(parse_id_with_thousands);
    RUN_TEST(parse_id_with_thousands_and_decimal);
    RUN_TEST(parse_id_four_decimals);
    RUN_TEST(parse_id_less_than_four_decimals);
    RUN_TEST(parse_id_negative);
    RUN_TEST(parse_id_with_whitespace);
    RUN_TEST(parse_id_zero);
    RUN_TEST(parse_id_zero_with_decimal);
    RUN_TEST(parse_id_null);
    RUN_TEST(parse_id_empty);
    RUN_TEST(parse_id_invalid_chars);
    RUN_TEST(parse_id_large_number);

    TEST_SUMMARY();

    return g_test_failed > 0 ? 1 : 0;
}
