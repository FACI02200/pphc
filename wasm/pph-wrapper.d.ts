/**
 * PPHC WebAssembly TypeScript Definitions
 */

export interface PPh21Input {
    /** Subject type (0=pegawai_tetap, 1=pensiunan, etc.) */
    subjectType: number;
    /** Monthly gross income */
    brutoMonthly: bigint;
    /** Number of months paid in the year */
    monthsPaid: number;
    /** Pension contribution deduction */
    pensionContribution?: bigint;
    /** Zakat or donation deduction */
    zakatOrDonation?: bigint;
    /** PTKP status (0=TK/0, 1=TK/1, ..., 7=K/3) */
    ptkpStatus: number;
    /** Calculation scheme (0=lama/progressive, 1=TER) */
    scheme: number;
    /** TER category (0=A, 1=B, 2=C) */
    terCategory: number;
    /** Array of bonuses */
    bonuses?: PPh21Bonus[];
}

export interface PPh21Bonus {
    /** Month when bonus is paid (1-12) */
    month: number;
    /** Bonus amount */
    amount: bigint;
    /** Bonus name (e.g., "THR", "Bonus Tahunan") */
    name: string;
}

export interface TaxBreakdownRow {
    /** Row label */
    label: string;
    /** Value (currency, percent, or text) */
    value: bigint | string;
    /** Value type (0=currency, 1=percent, 2=text) */
    valueType: number;
    /** Optional note */
    note?: string;
    /** Display variant (0=normal, 1=section, 2=subtotal, 3=total, 4=group, 5=spacer) */
    variant: number;
}

export interface TaxResult {
    /** Total tax amount */
    totalTax: bigint;
    /** Detailed breakdown of calculation */
    breakdown: TaxBreakdownRow[];
}

export class PPHCalculator {
    /**
     * Get library version
     */
    getVersion(): string;

    /**
     * Parse Indonesian number format to internal representation
     * @param str Number in Indonesian format (e.g., "1.234.567,89")
     * @returns Internal money value (bigint with 4 decimal places)
     */
    parseMoneyID(str: string): bigint;

    /**
     * Format money value to display string
     * @param value Internal money value
     * @returns Formatted string (e.g., "1,234,567.8900")
     */
    formatMoney(value: bigint): string;

    /**
     * Create money value from whole and fractional parts
     * @param whole Whole part
     * @param frac Fractional part (0-9999, representing 4 decimal places)
     * @returns Internal money value
     * @example
     * createMoney(100, 5000) // Returns 100.5000
     */
    createMoney(whole: number, frac?: number): bigint;

    /**
     * Calculate PPh 21 (Income Tax Article 21/26)
     * @param input PPh21 input parameters
     * @returns Calculation result with breakdown
     */
    calculatePPh21(input: PPh21Input): TaxResult;

    /**
     * Add two money values
     */
    moneyAdd(a: bigint, b: bigint): bigint;

    /**
     * Subtract two money values
     */
    moneySub(a: bigint, b: bigint): bigint;
}

/**
 * Creates and initializes the PPH calculator module
 * @param wasmPath Path to the pph.js file
 */
export function createPPH(wasmPath?: string): Promise<PPHCalculator>;

/**
 * Subject types for PPh21
 */
export enum PPh21SubjectType {
    PEGAWAI_TETAP = 0,
    PENSIUNAN = 1,
    PEGAWAI_TIDAK_TETAP = 2,
    BUKAN_PEGAWAI = 3,
    PESERTA_KEGIATAN = 4,
    PROGRAM_PENSIUN = 5,
    MANTAN_PEGAWAI = 6,
    WPLN = 7
}

/**
 * Calculation schemes for PPh21
 */
export enum PPh21Scheme {
    LAMA = 0,  // Progressive (Pasal 17)
    TER = 1    // Tarif Efektif Rata-rata
}

/**
 * TER categories
 */
export enum PPh21TERCategory {
    A = 0,  // No NPWP or Category A PTKP
    B = 1,  // Category B PTKP (TK/2, TK/3, K/1)
    C = 2   // Category C PTKP (K/2, K/3)
}

/**
 * PTKP status codes
 */
export enum PTKPStatus {
    TK0 = 0,  // Single, no dependents
    TK1 = 1,  // Single, 1 dependent
    TK2 = 2,  // Single, 2 dependents
    TK3 = 3,  // Single, 3 dependents
    K0 = 4,   // Married, no dependents
    K1 = 5,   // Married, 1 dependent
    K2 = 6,   // Married, 2 dependents
    K3 = 7    // Married, 3 dependents
}
