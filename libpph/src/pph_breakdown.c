/*
 * PPH Breakdown - Breakdown generation and management
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <pph/pph_calculator.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Initial capacity for breakdown array */
#define INITIAL_BREAKDOWN_CAPACITY 64

/* ============================================
   Result Management
   ============================================ */

pph_result_t* pph_result_create(void) {
    pph_result_t *result;

    result = (pph_result_t*)malloc(sizeof(pph_result_t));
    if (result == NULL) {
        return NULL;
    }

    result->total_tax = PPH_ZERO;
    result->breakdown_count = 0;
    result->breakdown_capacity = INITIAL_BREAKDOWN_CAPACITY;

    result->breakdown = (pph_breakdown_row_t*)malloc(
        sizeof(pph_breakdown_row_t) * result->breakdown_capacity);

    if (result->breakdown == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

void pph_result_free(pph_result_t *result) {
    if (result == NULL) {
        return;
    }

    if (result->breakdown != NULL) {
        free(result->breakdown);
    }

    free(result);
}

/* ============================================
   Breakdown Row Management
   ============================================ */

static int pph_result_ensure_capacity(pph_result_t *result) {
    pph_breakdown_row_t *new_breakdown;
    pph_size_t new_capacity;

    if (result->breakdown_count < result->breakdown_capacity) {
        return 1;  /* Success */
    }

    /* Double the capacity */
    new_capacity = result->breakdown_capacity * 2;
    new_breakdown = (pph_breakdown_row_t*)realloc(
        result->breakdown,
        sizeof(pph_breakdown_row_t) * new_capacity);

    if (new_breakdown == NULL) {
        return 0;  /* Failure */
    }

    result->breakdown = new_breakdown;
    result->breakdown_capacity = new_capacity;

    return 1;  /* Success */
}

int pph_result_add_row(pph_result_t *result,
                       const char *label,
                       pph_money_t value,
                       pph_value_type_t value_type,
                       const char *note,
                       pph_breakdown_variant_t variant) {
    pph_breakdown_row_t *row;

    if (result == NULL) {
        return 0;
    }

    if (!pph_result_ensure_capacity(result)) {
        return 0;
    }

    row = &result->breakdown[result->breakdown_count];

    /* Copy label */
    strncpy(row->label, label ? label : "", sizeof(row->label) - 1);
    row->label[sizeof(row->label) - 1] = '\0';

    /* Set value */
    row->value = value;
    row->value_type = value_type;

    /* Copy note */
    strncpy(row->note, note ? note : "", sizeof(row->note) - 1);
    row->note[sizeof(row->note) - 1] = '\0';

    /* Set variant */
    row->variant = variant;

    result->breakdown_count++;

    return 1;  /* Success */
}

int pph_result_add_section(pph_result_t *result, const char *label) {
    return pph_result_add_row(result, label, PPH_ZERO, PPH_VALUE_TEXT, NULL, PPH_BREAKDOWN_SECTION);
}

int pph_result_add_currency(pph_result_t *result, const char *label, pph_money_t value, const char *note) {
    return pph_result_add_row(result, label, value, PPH_VALUE_CURRENCY, note, PPH_BREAKDOWN_NORMAL);
}

int pph_result_add_percent(pph_result_t *result, const char *label, pph_money_t percent, const char *note) {
    return pph_result_add_row(result, label, percent, PPH_VALUE_PERCENT, note, PPH_BREAKDOWN_NORMAL);
}

int pph_result_add_subtotal(pph_result_t *result, const char *label, pph_money_t value) {
    return pph_result_add_row(result, label, value, PPH_VALUE_CURRENCY, NULL, PPH_BREAKDOWN_SUBTOTAL);
}

int pph_result_add_total(pph_result_t *result, const char *label, pph_money_t value) {
    return pph_result_add_row(result, label, value, PPH_VALUE_CURRENCY, NULL, PPH_BREAKDOWN_TOTAL);
}

int pph_result_add_spacer(pph_result_t *result) {
    return pph_result_add_row(result, "", PPH_ZERO, PPH_VALUE_TEXT, NULL, PPH_BREAKDOWN_SPACER);
}

/* ============================================
   Library Initialization
   ============================================ */

static const char *last_error = NULL;

void pph_init(void) {
    /* Currently no initialization needed */
    last_error = NULL;
}

const char* pph_get_last_error(void) {
    return last_error ? last_error : "No error";
}

void pph_set_last_error(const char *error) {
    last_error = error;
}

const char* pph_get_version(void) {
    return PPH_VERSION_STRING;
}
