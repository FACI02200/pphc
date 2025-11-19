/*
 * PPHC JNI Wrapper
 * Java Native Interface bindings for Android
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <jni.h>
#include <pph/pph_calculator.h>
#include <stdlib.h>
#include <string.h>

/* ============================================
   Helper Functions
   ============================================ */

static jstring create_jstring(JNIEnv *env, const char *str) {
    if (str == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, str);
}

static char* get_cstring(JNIEnv *env, jstring jstr) {
    const char *str;
    char *result;
    jsize len;

    if (jstr == NULL) {
        return NULL;
    }

    str = (*env)->GetStringUTFChars(env, jstr, NULL);
    if (str == NULL) {
        return NULL;
    }

    len = (*env)->GetStringUTFLength(env, jstr);
    result = (char*)malloc(len + 1);
    if (result != NULL) {
        strcpy(result, str);
    }

    (*env)->ReleaseStringUTFChars(env, jstr, str);
    return result;
}

/* ============================================
   Library Initialization
   ============================================ */

JNIEXPORT void JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeInit(JNIEnv *env, jclass clazz) {
    pph_init();
}

JNIEXPORT jstring JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeGetVersion(JNIEnv *env, jclass clazz) {
    const char *version = pph_get_version();
    return create_jstring(env, version);
}

JNIEXPORT jstring JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeGetLastError(JNIEnv *env, jclass clazz) {
    const char *error = pph_get_last_error();
    return create_jstring(env, error);
}

/* ============================================
   Money Operations
   ============================================ */

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeCreate(JNIEnv *env, jclass clazz, jlong whole, jint frac) {
    pph_money_t money = PPH_MONEY(whole, frac);
    return money.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeFromRupiah(JNIEnv *env, jclass clazz, jlong whole) {
    pph_money_t money = PPH_RUPIAH(whole);
    return money.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeParseIndonesian(JNIEnv *env, jclass clazz, jstring str) {
    char *cstr = get_cstring(env, str);
    pph_money_t money;

    if (cstr == NULL) {
        return 0;
    }

    money = pph_money_from_string_id(cstr);
    free(cstr);

    return money.value;
}

JNIEXPORT jstring JNICALL
Java_com_openpajak_pph_PPHMoney_nativeFormat(JNIEnv *env, jclass clazz, jlong value) {
    pph_money_t money;
    char buffer[64];

    money.value = value;
    pph_money_to_string_formatted(money, buffer, sizeof(buffer));

    return create_jstring(env, buffer);
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeAdd(JNIEnv *env, jclass clazz, jlong a, jlong b) {
    pph_money_t ma, mb, result;
    ma.value = a;
    mb.value = b;
    result = pph_money_add(ma, mb);
    return result.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeSub(JNIEnv *env, jclass clazz, jlong a, jlong b) {
    pph_money_t ma, mb, result;
    ma.value = a;
    mb.value = b;
    result = pph_money_sub(ma, mb);
    return result.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeMul(JNIEnv *env, jclass clazz, jlong a, jlong b) {
    pph_money_t ma, mb, result;
    ma.value = a;
    mb.value = b;
    result = pph_money_mul(ma, mb);
    return result.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativeDiv(JNIEnv *env, jclass clazz, jlong a, jlong divisor) {
    pph_money_t ma, result;
    ma.value = a;
    result = pph_money_div(ma, divisor);
    return result.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHMoney_nativePercent(JNIEnv *env, jclass clazz, jlong amount, jlong num, jlong den) {
    pph_money_t ma, result;
    ma.value = amount;
    result = pph_money_percent(ma, num, den);
    return result.value;
}

JNIEXPORT jint JNICALL
Java_com_openpajak_pph_PPHMoney_nativeCompare(JNIEnv *env, jclass clazz, jlong a, jlong b) {
    pph_money_t ma, mb;
    ma.value = a;
    mb.value = b;
    return pph_money_cmp(ma, mb);
}

/* ============================================
   PPh21 Calculation
   ============================================ */

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeCalculate(
    JNIEnv *env,
    jclass clazz,
    jint subjectType,
    jlong brutoMonthly,
    jint monthsPaid,
    jlong pensionContribution,
    jlong zakatOrDonation,
    jint ptkpStatus,
    jint scheme,
    jint terCategory,
    jlongArray bonusMonths,
    jlongArray bonusAmounts,
    jobjectArray bonusNames
) {
    pph21_input_t input = {0};
    pph_result_t *result;
    pph21_bonus_t *bonuses = NULL;
    jsize bonus_count = 0;
    int i;

    /* Set input fields */
    input.subject_type = (pph21_subject_type_t)subjectType;
    input.bruto_monthly.value = brutoMonthly;
    input.months_paid = monthsPaid;
    input.pension_contribution.value = pensionContribution;
    input.zakat_or_donation.value = zakatOrDonation;
    input.ptkp_status = (pph_ptkp_status_t)ptkpStatus;
    input.scheme = (pph21_scheme_t)scheme;
    input.ter_category = (pph21_ter_category_t)terCategory;

    /* Process bonuses if provided */
    if (bonusMonths != NULL && bonusAmounts != NULL && bonusNames != NULL) {
        bonus_count = (*env)->GetArrayLength(env, bonusMonths);
        if (bonus_count > 0) {
            jlong *months = (*env)->GetLongArrayElements(env, bonusMonths, NULL);
            jlong *amounts = (*env)->GetLongArrayElements(env, bonusAmounts, NULL);

            bonuses = (pph21_bonus_t*)malloc(sizeof(pph21_bonus_t) * bonus_count);
            if (bonuses != NULL) {
                for (i = 0; i < bonus_count; i++) {
                    jstring jname = (jstring)(*env)->GetObjectArrayElement(env, bonusNames, i);
                    char *name = get_cstring(env, jname);

                    bonuses[i].month = (int)months[i];
                    bonuses[i].amount.value = amounts[i];
                    if (name != NULL) {
                        strncpy(bonuses[i].name, name, sizeof(bonuses[i].name) - 1);
                        bonuses[i].name[sizeof(bonuses[i].name) - 1] = '\0';
                        free(name);
                    } else {
                        bonuses[i].name[0] = '\0';
                    }

                    (*env)->DeleteLocalRef(env, jname);
                }

                input.bonuses = bonuses;
                input.bonus_count = bonus_count;
            }

            (*env)->ReleaseLongArrayElements(env, bonusMonths, months, JNI_ABORT);
            (*env)->ReleaseLongArrayElements(env, bonusAmounts, amounts, JNI_ABORT);
        }
    }

    /* Calculate */
    result = pph21_calculate(&input);

    /* Clean up bonuses */
    if (bonuses != NULL) {
        free(bonuses);
    }

    /* Return result pointer as jlong */
    return (jlong)(intptr_t)result;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeGetTotalTax(JNIEnv *env, jclass clazz, jlong resultPtr) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result == NULL) {
        return 0;
    }
    return result->total_tax.value;
}

JNIEXPORT jint JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeGetBreakdownCount(JNIEnv *env, jclass clazz, jlong resultPtr) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result == NULL) {
        return 0;
    }
    return (jint)result->breakdown_count;
}

JNIEXPORT jstring JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeGetBreakdownLabel(JNIEnv *env, jclass clazz, jlong resultPtr, jint index) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result == NULL || index < 0 || index >= (jint)result->breakdown_count) {
        return NULL;
    }
    return create_jstring(env, result->breakdown[index].label);
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeGetBreakdownValue(JNIEnv *env, jclass clazz, jlong resultPtr, jint index) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result == NULL || index < 0 || index >= (jint)result->breakdown_count) {
        return 0;
    }
    return result->breakdown[index].value.value;
}

JNIEXPORT jint JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeGetBreakdownVariant(JNIEnv *env, jclass clazz, jlong resultPtr, jint index) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result == NULL || index < 0 || index >= (jint)result->breakdown_count) {
        return 0;
    }
    return (jint)result->breakdown[index].variant;
}

JNIEXPORT void JNICALL
Java_com_openpajak_pph_PPH21Calculator_nativeFreeResult(JNIEnv *env, jclass clazz, jlong resultPtr) {
    pph_result_t *result = (pph_result_t*)(intptr_t)resultPtr;
    if (result != NULL) {
        pph_result_free(result);
    }
}

/* ============================================
   Utility Functions
   ============================================ */

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeGetPTKP(JNIEnv *env, jclass clazz, jint status) {
    pph_money_t ptkp = pph_get_ptkp((pph_ptkp_status_t)status);
    return ptkp.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeCalculatePasal17(JNIEnv *env, jclass clazz, jlong pkp) {
    pph_money_t pkp_money, tax;
    pkp_money.value = pkp;
    tax = pph_calculate_pasal17(pkp_money);
    return tax.value;
}

JNIEXPORT jlong JNICALL
Java_com_openpajak_pph_PPHCalculator_nativeGetTERBulananRate(JNIEnv *env, jclass clazz, jint category, jlong bruto) {
    pph_money_t bruto_money, rate;
    bruto_money.value = bruto;
    rate = pph_get_ter_bulanan_rate((pph21_ter_category_t)category, bruto_money);
    return rate.value;
}
