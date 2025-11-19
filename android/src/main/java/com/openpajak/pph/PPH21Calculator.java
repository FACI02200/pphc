package com.openpajak.pph;

import java.util.ArrayList;
import java.util.List;

/**
 * PPh21/26 Income Tax Calculator
 */
public class PPH21Calculator {

    /**
     * Calculate PPh21/26 tax
     * @param input Calculation input
     * @return Tax calculation result
     */
    public static PPH21Result calculate(PPH21Input input) {
        // Prepare bonus arrays
        long[] bonusMonths = null;
        long[] bonusAmounts = null;
        String[] bonusNames = null;

        if (input.bonuses != null && !input.bonuses.isEmpty()) {
            int size = input.bonuses.size();
            bonusMonths = new long[size];
            bonusAmounts = new long[size];
            bonusNames = new String[size];

            for (int i = 0; i < size; i++) {
                Bonus bonus = input.bonuses.get(i);
                bonusMonths[i] = bonus.month;
                bonusAmounts[i] = bonus.amount.getValue();
                bonusNames[i] = bonus.name;
            }
        }

        // Call native method
        long resultPtr = nativeCalculate(
            input.subjectType.ordinal(),
            input.brutoMonthly.getValue(),
            input.monthsPaid,
            input.pensionContribution != null ? input.pensionContribution.getValue() : 0,
            input.zakatOrDonation != null ? input.zakatOrDonation.getValue() : 0,
            input.ptkpStatus.ordinal(),
            input.scheme.ordinal(),
            input.terCategory.ordinal(),
            bonusMonths,
            bonusAmounts,
            bonusNames
        );

        if (resultPtr == 0) {
            throw new RuntimeException("PPh21 calculation failed: " + PPHCalculator.getLastError());
        }

        return new PPH21Result(resultPtr);
    }

    // Native methods
    private static native long nativeCalculate(
        int subjectType,
        long brutoMonthly,
        int monthsPaid,
        long pensionContribution,
        long zakatOrDonation,
        int ptkpStatus,
        int scheme,
        int terCategory,
        long[] bonusMonths,
        long[] bonusAmounts,
        String[] bonusNames
    );

    static native long nativeGetTotalTax(long resultPtr);
    static native int nativeGetBreakdownCount(long resultPtr);
    static native String nativeGetBreakdownLabel(long resultPtr, int index);
    static native long nativeGetBreakdownValue(long resultPtr, int index);
    static native int nativeGetBreakdownVariant(long resultPtr, int index);
    static native void nativeFreeResult(long resultPtr);

    /**
     * Subject types for PPh21
     */
    public enum SubjectType {
        PEGAWAI_TETAP,        // Permanent employee
        PENSIUNAN,            // Retiree
        PEGAWAI_TIDAK_TETAP,  // Non-permanent employee
        BUKAN_PEGAWAI,        // Non-employee
        PESERTA_KEGIATAN,     // Activity participant
        PROGRAM_PENSIUN,      // Pension withdrawal
        MANTAN_PEGAWAI,       // Former employee
        WPLN                  // Foreign tax subject (PPh 26)
    }

    /**
     * Calculation schemes
     */
    public enum Scheme {
        LAMA,  // Progressive (Pasal 17)
        TER    // Tarif Efektif Rata-rata
    }

    /**
     * Bonus entry for multi-bonus calculation
     */
    public static class Bonus {
        public final int month;          // Month (1-12)
        public final PPHMoney amount;    // Bonus amount
        public final String name;        // Bonus name (e.g., "THR", "Quarterly Bonus")

        public Bonus(int month, PPHMoney amount, String name) {
            this.month = month;
            this.amount = amount;
            this.name = name;
        }
    }

    /**
     * PPh21 calculation input
     */
    public static class PPH21Input {
        public SubjectType subjectType = SubjectType.PEGAWAI_TETAP;
        public PPHMoney brutoMonthly;
        public int monthsPaid = 12;
        public PPHMoney pensionContribution;
        public PPHMoney zakatOrDonation;
        public PPHCalculator.PTKPStatus ptkpStatus = PPHCalculator.PTKPStatus.TK0;
        public Scheme scheme = Scheme.LAMA;
        public PPHCalculator.TERCategory terCategory = PPHCalculator.TERCategory.A;
        public List<Bonus> bonuses;

        public PPH21Input(PPHMoney brutoMonthly) {
            this.brutoMonthly = brutoMonthly;
        }

        public PPH21Input withMonthsPaid(int months) {
            this.monthsPaid = months;
            return this;
        }

        public PPH21Input withPTKPStatus(PPHCalculator.PTKPStatus status) {
            this.ptkpStatus = status;
            return this;
        }

        public PPH21Input withScheme(Scheme scheme) {
            this.scheme = scheme;
            return this;
        }

        public PPH21Input withTERCategory(PPHCalculator.TERCategory category) {
            this.terCategory = category;
            return this;
        }

        public PPH21Input withPensionContribution(PPHMoney amount) {
            this.pensionContribution = amount;
            return this;
        }

        public PPH21Input withZakatOrDonation(PPHMoney amount) {
            this.zakatOrDonation = amount;
            return this;
        }

        public PPH21Input withBonus(int month, PPHMoney amount, String name) {
            if (bonuses == null) {
                bonuses = new ArrayList<>();
            }
            bonuses.add(new Bonus(month, amount, name));
            return this;
        }
    }

    /**
     * Breakdown row variant
     */
    public enum BreakdownVariant {
        NORMAL,    // Normal row
        SECTION,   // Section header
        SUBTOTAL,  // Subtotal row
        TOTAL,     // Total row
        GROUP,     // Group header
        SPACER     // Visual spacer
    }

    /**
     * Breakdown row
     */
    public static class BreakdownRow {
        public final String label;
        public final PPHMoney value;
        public final BreakdownVariant variant;

        BreakdownRow(String label, PPHMoney value, BreakdownVariant variant) {
            this.label = label;
            this.value = value;
            this.variant = variant;
        }

        @Override
        public String toString() {
            return label + ": " + value.toString();
        }
    }

    /**
     * PPh21 calculation result
     */
    public static class PPH21Result implements AutoCloseable {
        private long resultPtr;
        private PPHMoney totalTax;
        private List<BreakdownRow> breakdown;

        PPH21Result(long resultPtr) {
            this.resultPtr = resultPtr;
            this.totalTax = PPHMoney.fromValue(nativeGetTotalTax(resultPtr));

            // Load breakdown
            int count = nativeGetBreakdownCount(resultPtr);
            this.breakdown = new ArrayList<>(count);

            for (int i = 0; i < count; i++) {
                String label = nativeGetBreakdownLabel(resultPtr, i);
                long value = nativeGetBreakdownValue(resultPtr, i);
                int variantOrdinal = nativeGetBreakdownVariant(resultPtr, i);

                BreakdownVariant variant = BreakdownVariant.values()[variantOrdinal];
                breakdown.add(new BreakdownRow(label, PPHMoney.fromValue(value), variant));
            }
        }

        /**
         * Get total tax amount
         * @return Total tax
         */
        public PPHMoney getTotalTax() {
            return totalTax;
        }

        /**
         * Get calculation breakdown
         * @return List of breakdown rows
         */
        public List<BreakdownRow> getBreakdown() {
            return breakdown;
        }

        /**
         * Free native resources
         */
        @Override
        public void close() {
            if (resultPtr != 0) {
                nativeFreeResult(resultPtr);
                resultPtr = 0;
            }
        }

        @Override
        protected void finalize() throws Throwable {
            close();
            super.finalize();
        }
    }
}
