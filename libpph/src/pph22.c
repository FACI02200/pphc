/*
 * PPH22 - PPh 22 Calculator
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <pph/pph_calculator.h>

extern pph_result_t* pph_result_create(void);
extern int pph_result_add_section(pph_result_t *result, const char *label);
extern int pph_result_add_currency(pph_result_t *result, const char *label, pph_money_t value, const char *note);
extern int pph_result_add_percent(pph_result_t *result, const char *label, pph_money_t percent, const char *note);
extern int pph_result_add_total(pph_result_t *result, const char *label, pph_money_t value);
extern void pph_set_last_error(const char *error);

pph_result_t* pph22_calculate(const pph22_input_t *input) {
    pph_result_t *result;
    pph_money_t tax;

    if (input == NULL) {
        pph_set_last_error("Input is NULL");
        return NULL;
    }

    result = pph_result_create();
    if (!result) {
        pph_set_last_error("Memory allocation failed");
        return NULL;
    }

    tax = pph_money_mul(input->dpp, input->rate);

    pph_result_add_section(result, "PPh 22");
    pph_result_add_currency(result, "DPP", input->dpp, NULL);
    pph_result_add_percent(result, "Tarif", input->rate, NULL);
    pph_result_add_total(result, "PPh 22", tax);

    result->total_tax = tax;
    return result;
}
