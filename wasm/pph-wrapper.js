/**
 * PPHC WebAssembly Wrapper
 * High-level JavaScript API for Indonesian Tax Calculator
 *
 * Usage:
 *   import { createPPH } from './pph-wrapper.js';
 *   const pph = await createPPH('./pph.js');
 *   const result = pph.calculatePPh21({ ... });
 */

/**
 * Creates and initializes the PPH calculator module
 * @param {string} wasmPath - Path to the pph.js file
 * @returns {Promise<PPHCalculator>}
 */
export async function createPPH(wasmPath = './pph.js') {
    const moduleFactory = await import(wasmPath);
    const module = await moduleFactory.default();

    // Initialize the library
    module._pph_init();

    return new PPHCalculator(module);
}

/**
 * PPH Calculator wrapper class
 */
class PPHCalculator {
    constructor(module) {
        this.module = module;
        this.textDecoder = new TextDecoder('utf-8');
    }

    /**
     * Get library version
     * @returns {string}
     */
    getVersion() {
        const ptr = this.module._pph_get_version();
        return this.module.UTF8ToString(ptr);
    }

    /**
     * Parse Indonesian number format to internal representation
     * @param {string} str - Number in Indonesian format (e.g., "1.234.567,89")
     * @returns {bigint} Internal money value
     */
    parseMoneyID(str) {
        const strPtr = this.allocateString(str);
        const result = this.module._pph_money_from_string_id(strPtr);
        this.module._free(strPtr);
        return result;
    }

    /**
     * Format money value to Indonesian format
     * @param {bigint} value - Internal money value
     * @returns {string} Formatted string (e.g., "1,234,567.8900")
     */
    formatMoney(value) {
        const bufferSize = 64;
        const buffer = this.module._malloc(bufferSize);

        // Create a money struct (8 bytes for int64_t)
        const moneyPtr = this.module._malloc(8);
        this.module.setValue(moneyPtr, value, 'i64');

        this.module._pph_money_to_string_formatted(moneyPtr, buffer, bufferSize);
        const result = this.module.UTF8ToString(buffer);

        this.module._free(buffer);
        this.module._free(moneyPtr);

        return result;
    }

    /**
     * Create money value from whole and fractional parts
     * @param {number} whole - Whole part
     * @param {number} frac - Fractional part (0-9999)
     * @returns {bigint} Internal money value
     */
    createMoney(whole, frac = 0) {
        return BigInt(whole) * 10000n + BigInt(frac);
    }

    /**
     * Calculate PPh 21 (Income Tax Article 21/26)
     * @param {Object} input - PPh21 input parameters
     * @returns {Object} Calculation result with breakdown
     */
    calculatePPh21(input) {
        // Allocate input struct
        const inputSize = 128; // Approximate size of pph21_input_t
        const inputPtr = this.module._malloc(inputSize);

        // Set input fields (simplified - needs proper struct layout)
        // This is a simplified example; real implementation needs proper struct handling

        // For now, return error indicating this needs proper implementation
        this.module._free(inputPtr);
        throw new Error('PPh21 calculation not yet fully implemented in wrapper. Please use the C API directly.');
    }

    /**
     * Helper: Allocate C string from JS string
     * @private
     */
    allocateString(str) {
        const length = this.module.lengthBytesUTF8(str) + 1;
        const ptr = this.module._malloc(length);
        this.module.stringToUTF8(str, ptr, length);
        return ptr;
    }

    /**
     * Add two money values
     * @param {bigint} a
     * @param {bigint} b
     * @returns {bigint}
     */
    moneyAdd(a, b) {
        const aPtr = this.createMoneyStruct(a);
        const bPtr = this.createMoneyStruct(b);
        const result = this.module._pph_money_add(aPtr, bPtr);
        this.module._free(aPtr);
        this.module._free(bPtr);
        return this.module.getValue(result, 'i64');
    }

    /**
     * Subtract two money values
     * @param {bigint} a
     * @param {bigint} b
     * @returns {bigint}
     */
    moneySub(a, b) {
        const aPtr = this.createMoneyStruct(a);
        const bPtr = this.createMoneyStruct(b);
        const result = this.module._pph_money_sub(aPtr, bPtr);
        this.module._free(aPtr);
        this.module._free(bPtr);
        return this.module.getValue(result, 'i64');
    }

    /**
     * Helper: Create money struct in WASM memory
     * @private
     */
    createMoneyStruct(value) {
        const ptr = this.module._malloc(8);
        this.module.setValue(ptr, value, 'i64');
        return ptr;
    }
}

// Export for Node.js/CommonJS
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { createPPH };
}
