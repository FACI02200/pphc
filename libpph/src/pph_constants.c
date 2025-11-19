/*
 * PPH Constants - Tax tables and rates
 * Copyright (c) 2025 OpenPajak Contributors
 */

#include <pph/pph_calculator.h>

/* ============================================
   PTKP Table (Penghasilan Tidak Kena Pajak)
   ============================================ */

static const pph_money_t PPH_PTKP_TABLE[8] = {
    PPH_RUPIAH(54000000),  /* TK/0 */
    PPH_RUPIAH(58500000),  /* TK/1 */
    PPH_RUPIAH(63000000),  /* TK/2 */
    PPH_RUPIAH(67500000),  /* TK/3 */
    PPH_RUPIAH(58500000),  /* K/0 */
    PPH_RUPIAH(63000000),  /* K/1 */
    PPH_RUPIAH(67500000),  /* K/2 */
    PPH_RUPIAH(72000000)   /* K/3 */
};

pph_money_t pph_get_ptkp(pph_ptkp_status_t status) {
    if (status < 0 || status > 7) {
        return PPH_PTKP_TABLE[0];  /* Default to TK/0 */
    }
    return PPH_PTKP_TABLE[status];
}

/* ============================================
   Pasal 17 Progressive Tax Layers
   ============================================ */

typedef struct {
    pph_money_t limit;
    pph_money_t rate;  /* Stored as fraction (0.05 = 500/10000) */
} pasal17_layer_t;

#define PASAL17_LAYER_COUNT 5

static const pasal17_layer_t PPH_PASAL17_LAYERS[PASAL17_LAYER_COUNT] = {
    { PPH_RUPIAH(60000000),    PPH_MONEY(0, 500) },   /* 5% = 0.0500 */
    { PPH_RUPIAH(190000000),   PPH_MONEY(0, 1500) },  /* 15% = 0.1500 */
    { PPH_RUPIAH(250000000),   PPH_MONEY(0, 2500) },  /* 25% = 0.2500 */
    { PPH_RUPIAH(4500000000),  PPH_MONEY(0, 3000) },  /* 30% = 0.3000 */
    { PPH_RUPIAH(2147483647),  PPH_MONEY(0, 3500) }   /* 35% = 0.3500, use max int as infinity */
};

pph_money_t pph_calculate_pasal17(pph_money_t pkp) {
    pph_money_t tax = PPH_ZERO;
    pph_money_t remaining = pkp;
    int i;

    for (i = 0; i < PASAL17_LAYER_COUNT; i++) {
        pph_money_t taxable, layer_tax;

        if (remaining.value <= 0) {
            break;
        }

        taxable = pph_money_min(remaining, PPH_PASAL17_LAYERS[i].limit);
        layer_tax = pph_money_mul(taxable, PPH_PASAL17_LAYERS[i].rate);

        tax = pph_money_add(tax, layer_tax);
        remaining = pph_money_sub(remaining, taxable);
    }

    return tax;
}

/* ============================================
   TER Bulanan (Monthly) Tables
   ============================================ */

typedef struct {
    pph_money_t ceiling;
    pph_money_t rate;
} ter_entry_t;

#define TER_BULANAN_A_COUNT 44
#define TER_BULANAN_B_COUNT 40
#define TER_BULANAN_C_COUNT 41

static const ter_entry_t TER_BULANAN_A[TER_BULANAN_A_COUNT] = {
    { PPH_RUPIAH(5400000), PPH_MONEY(0, 0) },
    { PPH_RUPIAH(5650000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(5950000), PPH_MONEY(0, 50) },
    { PPH_RUPIAH(6300000), PPH_MONEY(0, 75) },
    { PPH_RUPIAH(6750000), PPH_MONEY(0, 100) },
    { PPH_RUPIAH(7500000), PPH_MONEY(0, 125) },
    { PPH_RUPIAH(8550000), PPH_MONEY(0, 150) },
    { PPH_RUPIAH(9650000), PPH_MONEY(0, 175) },
    { PPH_RUPIAH(10050000), PPH_MONEY(0, 200) },
    { PPH_RUPIAH(10350000), PPH_MONEY(0, 225) },
    { PPH_RUPIAH(10700000), PPH_MONEY(0, 250) },
    { PPH_RUPIAH(11050000), PPH_MONEY(0, 300) },
    { PPH_RUPIAH(11600000), PPH_MONEY(0, 350) },
    { PPH_RUPIAH(12500000), PPH_MONEY(0, 400) },
    { PPH_RUPIAH(13750000), PPH_MONEY(0, 500) },
    { PPH_RUPIAH(15100000), PPH_MONEY(0, 600) },
    { PPH_RUPIAH(16950000), PPH_MONEY(0, 700) },
    { PPH_RUPIAH(19750000), PPH_MONEY(0, 800) },
    { PPH_RUPIAH(24150000), PPH_MONEY(0, 900) },
    { PPH_RUPIAH(26450000), PPH_MONEY(0, 1000) },
    { PPH_RUPIAH(28000000), PPH_MONEY(0, 1100) },
    { PPH_RUPIAH(30050000), PPH_MONEY(0, 1200) },
    { PPH_RUPIAH(32400000), PPH_MONEY(0, 1300) },
    { PPH_RUPIAH(35400000), PPH_MONEY(0, 1400) },
    { PPH_RUPIAH(39100000), PPH_MONEY(0, 1500) },
    { PPH_RUPIAH(43850000), PPH_MONEY(0, 1600) },
    { PPH_RUPIAH(47800000), PPH_MONEY(0, 1700) },
    { PPH_RUPIAH(51400000), PPH_MONEY(0, 1800) },
    { PPH_RUPIAH(56300000), PPH_MONEY(0, 1900) },
    { PPH_RUPIAH(62200000), PPH_MONEY(0, 2000) },
    { PPH_RUPIAH(68600000), PPH_MONEY(0, 2100) },
    { PPH_RUPIAH(77500000), PPH_MONEY(0, 2200) },
    { PPH_RUPIAH(89000000), PPH_MONEY(0, 2300) },
    { PPH_RUPIAH(103000000), PPH_MONEY(0, 2400) },
    { PPH_RUPIAH(125000000), PPH_MONEY(0, 2500) },
    { PPH_RUPIAH(157000000), PPH_MONEY(0, 2600) },
    { PPH_RUPIAH(206000000), PPH_MONEY(0, 2700) },
    { PPH_RUPIAH(337000000), PPH_MONEY(0, 2800) },
    { PPH_RUPIAH(454000000), PPH_MONEY(0, 2900) },
    { PPH_RUPIAH(550000000), PPH_MONEY(0, 3000) },
    { PPH_RUPIAH(695000000), PPH_MONEY(0, 3100) },
    { PPH_RUPIAH(910000000), PPH_MONEY(0, 3200) },
    { PPH_RUPIAH(1400000000), PPH_MONEY(0, 3300) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 3400) }  /* Infinity */
};

static const ter_entry_t TER_BULANAN_B[TER_BULANAN_B_COUNT] = {
    { PPH_RUPIAH(6200000), PPH_MONEY(0, 0) },
    { PPH_RUPIAH(6500000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(6850000), PPH_MONEY(0, 50) },
    { PPH_RUPIAH(7300000), PPH_MONEY(0, 75) },
    { PPH_RUPIAH(9200000), PPH_MONEY(0, 100) },
    { PPH_RUPIAH(10750000), PPH_MONEY(0, 150) },
    { PPH_RUPIAH(11250000), PPH_MONEY(0, 200) },
    { PPH_RUPIAH(11600000), PPH_MONEY(0, 250) },
    { PPH_RUPIAH(12600000), PPH_MONEY(0, 300) },
    { PPH_RUPIAH(13600000), PPH_MONEY(0, 400) },
    { PPH_RUPIAH(14950000), PPH_MONEY(0, 500) },
    { PPH_RUPIAH(16400000), PPH_MONEY(0, 600) },
    { PPH_RUPIAH(18450000), PPH_MONEY(0, 700) },
    { PPH_RUPIAH(21850000), PPH_MONEY(0, 800) },
    { PPH_RUPIAH(26000000), PPH_MONEY(0, 900) },
    { PPH_RUPIAH(27700000), PPH_MONEY(0, 1000) },
    { PPH_RUPIAH(29350000), PPH_MONEY(0, 1100) },
    { PPH_RUPIAH(31450000), PPH_MONEY(0, 1200) },
    { PPH_RUPIAH(33950000), PPH_MONEY(0, 1300) },
    { PPH_RUPIAH(37100000), PPH_MONEY(0, 1400) },
    { PPH_RUPIAH(41100000), PPH_MONEY(0, 1500) },
    { PPH_RUPIAH(45800000), PPH_MONEY(0, 1600) },
    { PPH_RUPIAH(49500000), PPH_MONEY(0, 1700) },
    { PPH_RUPIAH(53800000), PPH_MONEY(0, 1800) },
    { PPH_RUPIAH(58500000), PPH_MONEY(0, 1900) },
    { PPH_RUPIAH(64000000), PPH_MONEY(0, 2000) },
    { PPH_RUPIAH(71000000), PPH_MONEY(0, 2100) },
    { PPH_RUPIAH(80000000), PPH_MONEY(0, 2200) },
    { PPH_RUPIAH(93000000), PPH_MONEY(0, 2300) },
    { PPH_RUPIAH(109000000), PPH_MONEY(0, 2400) },
    { PPH_RUPIAH(129000000), PPH_MONEY(0, 2500) },
    { PPH_RUPIAH(163000000), PPH_MONEY(0, 2600) },
    { PPH_RUPIAH(211000000), PPH_MONEY(0, 2700) },
    { PPH_RUPIAH(374000000), PPH_MONEY(0, 2800) },
    { PPH_RUPIAH(459000000), PPH_MONEY(0, 2900) },
    { PPH_RUPIAH(555000000), PPH_MONEY(0, 3000) },
    { PPH_RUPIAH(704000000), PPH_MONEY(0, 3100) },
    { PPH_RUPIAH(957000000), PPH_MONEY(0, 3200) },
    { PPH_RUPIAH(1405000000), PPH_MONEY(0, 3300) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 3400) }
};

static const ter_entry_t TER_BULANAN_C[TER_BULANAN_C_COUNT] = {
    { PPH_RUPIAH(6600000), PPH_MONEY(0, 0) },
    { PPH_RUPIAH(6950000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(7350000), PPH_MONEY(0, 50) },
    { PPH_RUPIAH(7800000), PPH_MONEY(0, 75) },
    { PPH_RUPIAH(8850000), PPH_MONEY(0, 100) },
    { PPH_RUPIAH(9800000), PPH_MONEY(0, 125) },
    { PPH_RUPIAH(10950000), PPH_MONEY(0, 150) },
    { PPH_RUPIAH(11200000), PPH_MONEY(0, 175) },
    { PPH_RUPIAH(12050000), PPH_MONEY(0, 200) },
    { PPH_RUPIAH(12950000), PPH_MONEY(0, 300) },
    { PPH_RUPIAH(14150000), PPH_MONEY(0, 400) },
    { PPH_RUPIAH(15550000), PPH_MONEY(0, 500) },
    { PPH_RUPIAH(17050000), PPH_MONEY(0, 600) },
    { PPH_RUPIAH(19500000), PPH_MONEY(0, 700) },
    { PPH_RUPIAH(22700000), PPH_MONEY(0, 800) },
    { PPH_RUPIAH(26600000), PPH_MONEY(0, 900) },
    { PPH_RUPIAH(28100000), PPH_MONEY(0, 1000) },
    { PPH_RUPIAH(30100000), PPH_MONEY(0, 1100) },
    { PPH_RUPIAH(32600000), PPH_MONEY(0, 1200) },
    { PPH_RUPIAH(35400000), PPH_MONEY(0, 1300) },
    { PPH_RUPIAH(38900000), PPH_MONEY(0, 1400) },
    { PPH_RUPIAH(43000000), PPH_MONEY(0, 1500) },
    { PPH_RUPIAH(47400000), PPH_MONEY(0, 1600) },
    { PPH_RUPIAH(51200000), PPH_MONEY(0, 1700) },
    { PPH_RUPIAH(55800000), PPH_MONEY(0, 1800) },
    { PPH_RUPIAH(60400000), PPH_MONEY(0, 1900) },
    { PPH_RUPIAH(66700000), PPH_MONEY(0, 2000) },
    { PPH_RUPIAH(74500000), PPH_MONEY(0, 2100) },
    { PPH_RUPIAH(83200000), PPH_MONEY(0, 2200) },
    { PPH_RUPIAH(95600000), PPH_MONEY(0, 2300) },
    { PPH_RUPIAH(110000000), PPH_MONEY(0, 2400) },
    { PPH_RUPIAH(134000000), PPH_MONEY(0, 2500) },
    { PPH_RUPIAH(169000000), PPH_MONEY(0, 2600) },
    { PPH_RUPIAH(221000000), PPH_MONEY(0, 2700) },
    { PPH_RUPIAH(390000000), PPH_MONEY(0, 2800) },
    { PPH_RUPIAH(463000000), PPH_MONEY(0, 2900) },
    { PPH_RUPIAH(561000000), PPH_MONEY(0, 3000) },
    { PPH_RUPIAH(709000000), PPH_MONEY(0, 3100) },
    { PPH_RUPIAH(965000000), PPH_MONEY(0, 3200) },
    { PPH_RUPIAH(1419000000), PPH_MONEY(0, 3300) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 3400) }
};

pph_money_t pph_get_ter_bulanan_rate(pph21_ter_category_t category, pph_money_t bruto_monthly) {
    const ter_entry_t *table;
    int count, i;
    pph_money_t fallback_rate;

    switch (category) {
        case PPH21_TER_CATEGORY_B:
            table = TER_BULANAN_B;
            count = TER_BULANAN_B_COUNT;
            break;
        case PPH21_TER_CATEGORY_C:
            table = TER_BULANAN_C;
            count = TER_BULANAN_C_COUNT;
            break;
        case PPH21_TER_CATEGORY_A:
        default:
            table = TER_BULANAN_A;
            count = TER_BULANAN_A_COUNT;
            break;
    }

    fallback_rate = table[count - 1].rate;

    for (i = 0; i < count; i++) {
        if (pph_money_cmp(bruto_monthly, table[i].ceiling) <= 0) {
            return table[i].rate;
        }
    }

    return fallback_rate;
}

/* ============================================
   TER Harian (Daily) Tables
   ============================================ */

#define TER_HARIAN_COUNT 3

static const ter_entry_t TER_HARIAN_A[TER_HARIAN_COUNT] = {
    { PPH_RUPIAH(750000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(2500000), PPH_MONEY(0, 150) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 200) }
};

static const ter_entry_t TER_HARIAN_B[TER_HARIAN_COUNT] = {
    { PPH_RUPIAH(750000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(2500000), PPH_MONEY(0, 125) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 175) }
};

static const ter_entry_t TER_HARIAN_C[TER_HARIAN_COUNT] = {
    { PPH_RUPIAH(750000), PPH_MONEY(0, 25) },
    { PPH_RUPIAH(2500000), PPH_MONEY(0, 100) },
    { PPH_RUPIAH(2147483647), PPH_MONEY(0, 150) }
};

pph_money_t pph_get_ter_harian_rate(pph21_ter_category_t category, pph_money_t bruto) {
    const ter_entry_t *table;
    int i;

    switch (category) {
        case PPH21_TER_CATEGORY_B:
            table = TER_HARIAN_B;
            break;
        case PPH21_TER_CATEGORY_C:
            table = TER_HARIAN_C;
            break;
        case PPH21_TER_CATEGORY_A:
        default:
            table = TER_HARIAN_A;
            break;
    }

    for (i = 0; i < TER_HARIAN_COUNT; i++) {
        if (pph_money_cmp(bruto, table[i].ceiling) <= 0) {
            return table[i].rate;
        }
    }

    return table[TER_HARIAN_COUNT - 1].rate;
}
