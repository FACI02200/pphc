# PPHC WebAssembly

This directory contains WebAssembly build configurations and JavaScript/TypeScript wrappers for the PPHC library.

## Prerequisites

You need to install the Emscripten SDK:

```bash
# Clone emsdk
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest version
./emsdk install latest
./emsdk activate latest

# Set up environment (run this in each new terminal)
source ./emsdk_env.sh
```

## Building

From the project root directory:

```bash
# Build release version
./build-wasm.sh Release

# Build debug version with assertions
./build-wasm.sh Debug

# Clean build
./build-wasm.sh Release clean
```

Output files will be in `build-wasm/libpph/`:
- `pph.js` - JavaScript loader/glue code
- `pph.wasm` - Compiled WebAssembly binary

## Usage

### Basic Example (Node.js)

```javascript
const createPPHModule = require('./build-wasm/libpph/pph.js');

createPPHModule().then(module => {
    // Initialize library
    module._pph_init();

    // Get version
    const versionPtr = module._pph_get_version();
    console.log('PPHC Version:', module.UTF8ToString(versionPtr));

    // Parse Indonesian number format
    const strPtr = module.allocateUTF8('250.000.000,00');
    const money = module._pph_money_from_string_id(strPtr);
    module._free(strPtr);

    console.log('Parsed value:', money); // Internal representation

    // Format back to string
    const buffer = module._malloc(64);
    const moneyStruct = module._malloc(8);
    module.setValue(moneyStruct, money, 'i64');
    module._pph_money_to_string_formatted(moneyStruct, buffer, 64);
    console.log('Formatted:', module.UTF8ToString(buffer));

    module._free(buffer);
    module._free(moneyStruct);
});
```

### Using the High-Level Wrapper

```javascript
import { createPPH } from './wasm/pph-wrapper.js';

const pph = await createPPH('./build-wasm/libpph/pph.js');

console.log('Version:', pph.getVersion());

// Parse Indonesian format
const salary = pph.parseMoneyID('250.000.000,00');
console.log('Salary:', pph.formatMoney(salary));

// Money arithmetic
const bonus = pph.createMoney(100000000); // 100 million
const total = pph.moneyAdd(salary, bonus);
console.log('Total:', pph.formatMoney(total));
```

### Browser Usage

```html
<!DOCTYPE html>
<html>
<head>
    <title>PPHC WASM Demo</title>
</head>
<body>
    <h1>Indonesian Tax Calculator</h1>
    <div id="output"></div>

    <script type="module">
        import { createPPH } from './wasm/pph-wrapper.js';

        const pph = await createPPH('./pph.js');
        const output = document.getElementById('output');

        output.innerHTML = `
            <p>PPHC Version: ${pph.getVersion()}</p>
            <p>250M IDR formatted: ${pph.formatMoney(pph.parseMoneyID('250.000.000'))}</p>
        `;
    </script>
</body>
</html>
```

## API Reference

### Exported C Functions

All public API functions from `pph_calculator.h` are exported:

#### Initialization
- `_pph_init()` - Initialize library
- `_pph_get_version()` - Get version string
- `_pph_get_last_error()` - Get last error message

#### Money Operations
- `_pph_money_add(a, b)` - Add two money values
- `_pph_money_sub(a, b)` - Subtract
- `_pph_money_mul(a, b)` - Multiply
- `_pph_money_div(a, divisor)` - Divide
- `_pph_money_percent(amount, num, den)` - Calculate percentage
- `_pph_money_from_string_id(str)` - Parse Indonesian format
- `_pph_money_to_string_formatted(money, buffer, size)` - Format to string

#### Tax Calculations
- `_pph21_calculate(input)` - Calculate PPh 21/26
- `_pph22_calculate(input)` - Calculate PPh 22
- `_pph23_calculate(input)` - Calculate PPh 23
- `_pph4_2_calculate(input)` - Calculate PPh Final Pasal 4(2)
- `_ppn_calculate(input)` - Calculate PPN
- `_ppnbm_calculate(input)` - Calculate PPNBM

#### Utility Functions
- `_pph_get_ptkp(status)` - Get PTKP amount for status
- `_pph_calculate_pasal17(pkp)` - Calculate progressive tax
- `_pph_get_ter_bulanan_rate(category, bruto)` - Get monthly TER rate
- `_pph_get_ter_harian_rate(category, bruto)` - Get daily TER rate

### JavaScript Wrapper API

See `pph-wrapper.d.ts` for complete TypeScript definitions.

## Memory Management

The WASM module uses manual memory management. When calling C functions directly:

1. **Allocate strings**: Use `allocateUTF8()` or manually allocate with `_malloc()`
2. **Free memory**: Always call `_free()` on allocated pointers
3. **Struct handling**: Allocate proper sizes and use `getValue()`/`setValue()` for accessing fields

The high-level wrapper handles memory management automatically for most operations.

## Build Optimization

### Release Build (Optimized)
```bash
./build-wasm.sh Release
```
- Optimized with `-O3`
- Closure compiler enabled
- Assertions disabled
- Smaller bundle size

### Debug Build
```bash
./build-wasm.sh Debug
```
- No optimization (`-O0`)
- Source maps included
- Assertions and safe heap enabled
- Easier debugging

## Advanced Configuration

Edit `cmake/EmscriptenConfig.cmake` to customize:

- `WASM_ENABLE_SIMD` - Enable SIMD instructions (experimental)
- `WASM_ENABLE_THREADS` - Enable pthread support
- Exported functions list
- Runtime methods
- Memory growth settings

## Troubleshooting

### "Module is not defined"
Make sure you're loading the module correctly:
```javascript
// ESM
import createPPHModule from './pph.js';
const module = await createPPHModule();

// CommonJS
const createPPHModule = require('./pph.js');
createPPHModule().then(module => { ... });
```

### Memory leaks
Always free allocated memory:
```javascript
const ptr = module._malloc(size);
// ... use ptr ...
module._free(ptr); // Don't forget!
```

### Assertion failures
These indicate bugs or incorrect API usage. Run a debug build to get detailed error messages.

## Performance

The WASM build provides near-native performance:
- Fixed-point arithmetic is very fast in WASM
- Table lookups are efficient
- No garbage collection overhead for calculations

Benchmark (approximate):
- PPh21 calculation: ~0.1-0.5ms per calculation
- Money parsing: ~0.01ms per parse
- Money formatting: ~0.05ms per format

## License

Same as the main PPHC library. See LICENSE file in project root.
