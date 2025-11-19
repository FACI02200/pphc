package com.openpajak.pph;

/**
 * Fixed-point money type with 4 decimal places.
 * Internal representation: value * 10000
 *
 * Example: 100.5000 IDR = 1005000 internal units
 */
public class PPHMoney {
    private final long value;

    // Scale factor for 4 decimal places
    public static final long SCALE_FACTOR = 10000L;

    private PPHMoney(long value) {
        this.value = value;
    }

    /**
     * Create money from whole and fractional parts
     * @param whole Whole part (e.g., 100 for 100 rupiah)
     * @param frac Fractional part (0-9999, representing 4 decimal places)
     * @return PPHMoney instance
     */
    public static PPHMoney create(long whole, int frac) {
        long value = nativeCreate(whole, frac);
        return new PPHMoney(value);
    }

    /**
     * Create money from whole rupiah amount
     * @param whole Whole rupiah amount
     * @return PPHMoney instance
     */
    public static PPHMoney fromRupiah(long whole) {
        long value = nativeFromRupiah(whole);
        return new PPHMoney(value);
    }

    /**
     * Parse Indonesian format number string
     * @param str Number in Indonesian format (e.g., "1.234.567,89")
     * @return PPHMoney instance
     */
    public static PPHMoney parseIndonesian(String str) {
        long value = nativeParseIndonesian(str);
        return new PPHMoney(value);
    }

    /**
     * Create from raw internal value
     * @param value Internal value (value * 10000)
     * @return PPHMoney instance
     */
    public static PPHMoney fromValue(long value) {
        return new PPHMoney(value);
    }

    /**
     * Get internal value
     * @return Internal value (value * 10000)
     */
    public long getValue() {
        return value;
    }

    /**
     * Get whole part
     * @return Whole part (e.g., 100 for 100.5000)
     */
    public long getWhole() {
        return value / SCALE_FACTOR;
    }

    /**
     * Get fractional part
     * @return Fractional part (0-9999)
     */
    public int getFraction() {
        return (int)(Math.abs(value) % SCALE_FACTOR);
    }

    /**
     * Format to string with thousands separator
     * @return Formatted string (e.g., "1,234,567.8900")
     */
    public String format() {
        return nativeFormat(value);
    }

    /**
     * Add two money values
     * @param other Value to add
     * @return Result
     */
    public PPHMoney add(PPHMoney other) {
        long result = nativeAdd(this.value, other.value);
        return new PPHMoney(result);
    }

    /**
     * Subtract two money values
     * @param other Value to subtract
     * @return Result
     */
    public PPHMoney subtract(PPHMoney other) {
        long result = nativeSub(this.value, other.value);
        return new PPHMoney(result);
    }

    /**
     * Multiply by another money value
     * @param other Value to multiply by
     * @return Result
     */
    public PPHMoney multiply(PPHMoney other) {
        long result = nativeMul(this.value, other.value);
        return new PPHMoney(result);
    }

    /**
     * Divide by integer divisor
     * @param divisor Divisor
     * @return Result
     */
    public PPHMoney divide(long divisor) {
        long result = nativeDiv(this.value, divisor);
        return new PPHMoney(result);
    }

    /**
     * Calculate percentage
     * @param numerator Percentage numerator
     * @param denominator Percentage denominator
     * @return Result (amount * numerator / denominator)
     */
    public PPHMoney percent(long numerator, long denominator) {
        long result = nativePercent(this.value, numerator, denominator);
        return new PPHMoney(result);
    }

    /**
     * Compare with another money value
     * @param other Value to compare
     * @return -1 if less, 0 if equal, 1 if greater
     */
    public int compareTo(PPHMoney other) {
        return nativeCompare(this.value, other.value);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof PPHMoney)) return false;
        PPHMoney other = (PPHMoney) obj;
        return value == other.value;
    }

    @Override
    public int hashCode() {
        return Long.hashCode(value);
    }

    @Override
    public String toString() {
        return format() + " IDR";
    }

    // Native methods
    private static native long nativeCreate(long whole, int frac);
    private static native long nativeFromRupiah(long whole);
    private static native long nativeParseIndonesian(String str);
    private static native String nativeFormat(long value);
    private static native long nativeAdd(long a, long b);
    private static native long nativeSub(long a, long b);
    private static native long nativeMul(long a, long b);
    private static native long nativeDiv(long a, long divisor);
    private static native long nativePercent(long amount, long num, long den);
    private static native int nativeCompare(long a, long b);
}
