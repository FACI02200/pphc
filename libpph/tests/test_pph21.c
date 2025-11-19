/*
 * Test: PPh21 calculations
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <pph/pph_calculator.h>
#include "test_common.h"

int g_test_total = 0;
int g_test_passed = 0;
int g_test_failed = 0;

TEST(pph21_pegawai_tetap_basic) {
    pph21_input_t input = {0};
    pph_result_t *result;

    input.subject_type = PPH21_PEGAWAI_TETAP;
    input.bruto_monthly = PPH_RUPIAH(10000000);
    input.months_paid = 12;
    input.pension_contribution = PPH_RUPIAH(100000);
    input.ptkp_status = PPH_PTKP_TK0;
    input.scheme = PPH21_SCHEME_TER;
    input.ter_category = PPH21_TER_CATEGORY_A;

    result = pph21_calculate(&input);
    ASSERT_NOT_NULL(result);
    ASSERT_TRUE(result->total_tax.value > 0);
    ASSERT_TRUE(result->breakdown_count > 0);

    pph_result_free(result);
    return 0;
}

TEST(pph21_null_input) {
    pph_result_t *result;

    result = pph21_calculate(NULL);
    ASSERT_TRUE(result == NULL);

    return 0;
}

int main(void) {
    pph_init();

    printf("========================================\n");
    printf("  PPh21 Calculation Tests\n");
    printf("========================================\n\n");

    RUN_TEST(pph21_pegawai_tetap_basic);
    RUN_TEST(pph21_null_input);

    TEST_SUMMARY();

    return g_test_failed > 0 ? 1 : 0;
}
