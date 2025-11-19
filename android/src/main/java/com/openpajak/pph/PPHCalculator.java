package com.openpajak.pph;

/**
 * Main entry point for PPHC library.
 * Initialize this class before using any tax calculators.
 */
public class PPHCalculator {
    private static boolean initialized = false;

    static {
        System.loadLibrary("pph");
    }

    /**
     * Initialize the PPHC library.
     * Must be called before using any calculation methods.
     */
    public static synchronized void initialize() {
        if (!initialized) {
            nativeInit();
            initialized = true;
        }
    }

    /**
     * Get library version string
     * @return Version string (e.g., "0.1a")
     */
    public static String getVersion() {
        return nativeGetVersion();
    }

    /**
     * Get last error message
     * @return Error message or null if no error
     */
    public static String getLastError() {
        return nativeGetLastError();
    }

    /**
     * Get PTKP (Penghasilan Tidak Kena Pajak) amount for given status
     * @param status PTKP status
     * @return PTKP amount
     */
    public static PPHMoney getPTKP(PTKPStatus status) {
        long value = nativeGetPTKP(status.ordinal());
        return PPHMoney.fromValue(value);
    }

    /**
     * Calculate progressive tax using Pasal 17 rates
     * @param pkp Penghasilan Kena Pajak (taxable income)
     * @return Tax amount
     */
    public static PPHMoney calculatePasal17(PPHMoney pkp) {
        long value = nativeCalculatePasal17(pkp.getValue());
        return PPHMoney.fromValue(value);
    }

    /**
     * Get TER (Tarif Efektif Rata-rata) monthly rate
     * @param category TER category
     * @param brutoMonthly Gross monthly income
     * @return TER rate (as money value, e.g., 0.0500 for 5%)
     */
    public static PPHMoney getTERBulananRate(TERCategory category, PPHMoney brutoMonthly) {
        long value = nativeGetTERBulananRate(category.ordinal(), brutoMonthly.getValue());
        return PPHMoney.fromValue(value);
    }

    // Native methods
    private static native void nativeInit();
    private static native String nativeGetVersion();
    private static native String nativeGetLastError();
    private static native long nativeGetPTKP(int status);
    private static native long nativeCalculatePasal17(long pkp);
    private static native long nativeGetTERBulananRate(int category, long bruto);

    /**
     * PTKP (Penghasilan Tidak Kena Pajak) Status
     */
    public enum PTKPStatus {
        TK0, // Single, no dependents
        TK1, // Single, 1 dependent
        TK2, // Single, 2 dependents
        TK3, // Single, 3 dependents
        K0,  // Married, no dependents
        K1,  // Married, 1 dependent
        K2,  // Married, 2 dependents
        K3   // Married, 3 dependents
    }

    /**
     * TER (Tarif Efektif Rata-rata) Category
     */
    public enum TERCategory {
        A, // No NPWP or Category A PTKP (TK/0, TK/1, K/0)
        B, // Category B PTKP (TK/2, TK/3, K/1)
        C  // Category C PTKP (K/2, K/3)
    }
}
