# PPHC Android

Android Native Interface (JNI) bindings for the PPHC Indonesian Tax Calculator library.

## Prerequisites

1. **Android NDK** (r21 or later recommended)
   - Download from: https://developer.android.com/ndk/downloads
   - Or install via Android Studio SDK Manager

2. **CMake** (3.10 or later)
   - Usually included with Android Studio

3. **Environment Setup**
   ```bash
   export ANDROID_NDK=/path/to/android-ndk
   # Or on macOS with Android Studio:
   export ANDROID_NDK=$HOME/Library/Android/sdk/ndk/<version>
   ```

## Building

### Quick Build (All ABIs)

From the project root:

```bash
./build-android.sh Release
```

This builds for all Android ABIs:
- `arm64-v8a` (64-bit ARM, most modern devices)
- `armeabi-v7a` (32-bit ARM, older devices)
- `x86` (32-bit x86, emulators)
- `x86_64` (64-bit x86, emulators)

Output: `build-android/<ABI>/libpph/libpph.so`

### Build for Specific ABI

```bash
export ANDROID_NDK=/path/to/ndk

mkdir -p build-android/arm64-v8a
cd build-android/arm64-v8a

cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release

cmake --build . -j4
```

### Debug Build

```bash
./build-android.sh Debug
```

## Integration with Android Studio/Gradle

### 1. Copy Native Libraries

Copy the built `.so` files to your Android project:

```
your-app/
└── src/main/
    └── jniLibs/
        ├── arm64-v8a/
        │   └── libpph.so
        ├── armeabi-v7a/
        │   └── libpph.so
        ├── x86/
        │   └── libpph.so
        └── x86_64/
            └── libpph.so
```

### 2. Copy Java Sources

Copy Java files to your project:

```bash
cp -r android/src/main/java/com/openpajak/pph \
      your-app/src/main/java/com/openpajak/
```

Or add as a dependency in your `settings.gradle`:

```gradle
include ':pphc'
project(':pphc').projectDir = new File('../pphc/android')
```

### 3. Update build.gradle

In your app's `build.gradle`:

```gradle
android {
    defaultConfig {
        ndk {
            abiFilters 'arm64-v8a', 'armeabi-v7a', 'x86', 'x86_64'
        }
    }
}

dependencies {
    implementation project(':pphc')
    // Or if you copied the sources directly, no dependency needed
}
```

### 4. Initialize Library

In your Application class or main Activity:

```java
public class MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        PPHCalculator.initialize();
    }
}
```

## Usage Examples

### Basic Money Operations

```java
import com.openpajak.pph.*;

// Create money from rupiah
PPHMoney salary = PPHMoney.fromRupiah(10_000_000);  // 10 million IDR

// Parse Indonesian format
PPHMoney amount = PPHMoney.parseIndonesian("250.000.000,50");

// Arithmetic
PPHMoney bonus = PPHMoney.fromRupiah(5_000_000);
PPHMoney total = salary.add(bonus);

// Format for display
String formatted = total.format();  // "15,000,000.0000"
```

### PPh21 Calculation (Simple)

```java
// Initialize
PPHCalculator.initialize();

// Create input
PPHMoney salary = PPHMoney.fromRupiah(10_000_000);  // 10M/month

PPH21Calculator.PPH21Input input = new PPH21Calculator.PPH21Input(salary)
    .withMonthsPaid(12)
    .withPTKPStatus(PPHCalculator.PTKPStatus.TK0)
    .withScheme(PPH21Calculator.Scheme.TER)
    .withTERCategory(PPHCalculator.TERCategory.B);

// Calculate
try (PPH21Calculator.PPH21Result result = PPH21Calculator.calculate(input)) {
    PPHMoney totalTax = result.getTotalTax();
    Log.i("Tax", "Total tax: " + totalTax);

    // Show breakdown
    for (PPH21Calculator.BreakdownRow row : result.getBreakdown()) {
        Log.i("Tax", row.toString());
    }
}
```

### PPh21 with Bonuses

```java
PPHMoney salary = PPHMoney.fromRupiah(250_000_000);

PPH21Calculator.PPH21Input input = new PPH21Calculator.PPH21Input(salary)
    .withMonthsPaid(12)
    .withPTKPStatus(PPHCalculator.PTKPStatus.K2)
    .withScheme(PPH21Calculator.Scheme.TER)
    .withTERCategory(PPHCalculator.TERCategory.B)
    .withBonus(3, PPHMoney.fromRupiah(250_000_000), "THR")
    .withBonus(4, PPHMoney.fromRupiah(100_000_000), "Bonus Tahunan");

try (PPH21Calculator.PPH21Result result = PPH21Calculator.calculate(input)) {
    // Use result
    displayTaxBreakdown(result);
}
```

### Android UI Example

```java
public class TaxCalculatorActivity extends AppCompatActivity {
    private EditText inputSalary;
    private TextView outputTax;
    private RecyclerView breakdownList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tax_calculator);

        PPHCalculator.initialize();

        findViewById(R.id.btnCalculate).setOnClickListener(v -> calculateTax());
    }

    private void calculateTax() {
        String salaryStr = inputSalary.getText().toString();
        PPHMoney salary = PPHMoney.parseIndonesian(salaryStr);

        PPH21Calculator.PPH21Input input = new PPH21Calculator.PPH21Input(salary)
            .withMonthsPaid(12)
            .withPTKPStatus(PPHCalculator.PTKPStatus.TK0)
            .withScheme(PPH21Calculator.Scheme.TER)
            .withTERCategory(PPHCalculator.TERCategory.B);

        try (PPH21Calculator.PPH21Result result = PPH21Calculator.calculate(input)) {
            outputTax.setText(result.getTotalTax().toString());

            // Display breakdown in RecyclerView
            BreakdownAdapter adapter = new BreakdownAdapter(result.getBreakdown());
            breakdownList.setAdapter(adapter);
        } catch (Exception e) {
            Toast.makeText(this, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }
}
```

## API Reference

### PPHCalculator

Main entry point:
- `initialize()` - Initialize library (call once)
- `getVersion()` - Get library version
- `getPTKP(PTKPStatus)` - Get PTKP amount
- `calculatePasal17(PPHMoney)` - Calculate progressive tax

### PPHMoney

Fixed-point money with 4 decimal places:
- `fromRupiah(long)` - Create from whole rupiah
- `parseIndonesian(String)` - Parse "1.234.567,89" format
- `format()` - Format to display string
- `add(PPHMoney)` - Addition
- `subtract(PPHMoney)` - Subtraction
- `multiply(PPHMoney)` - Multiplication
- `divide(long)` - Division
- `percent(long, long)` - Calculate percentage

### PPH21Calculator

Income tax calculator:
- `calculate(PPH21Input)` - Calculate PPh21
- Input builder methods:
  - `withMonthsPaid(int)`
  - `withPTKPStatus(PTKPStatus)`
  - `withScheme(Scheme)`
  - `withTERCategory(TERCategory)`
  - `withBonus(int, PPHMoney, String)`

Result:
- `getTotalTax()` - Get total tax amount
- `getBreakdown()` - Get detailed breakdown

## Enums

### PTKPStatus
`TK0, TK1, TK2, TK3, K0, K1, K2, K3`

### SubjectType
`PEGAWAI_TETAP, PENSIUNAN, PEGAWAI_TIDAK_TETAP, BUKAN_PEGAWAI, PESERTA_KEGIATAN, PROGRAM_PENSIUN, MANTAN_PEGAWAI, WPLN`

### Scheme
`LAMA` (Progressive), `TER` (Simplified withholding)

### TERCategory
`A, B, C`

## Memory Management

`PPH21Result` implements `AutoCloseable`. Always use try-with-resources:

```java
try (PPH21Calculator.PPH21Result result = PPH21Calculator.calculate(input)) {
    // Use result
} // Automatically freed
```

Or manually:
```java
PPH21Calculator.PPH21Result result = PPH21Calculator.calculate(input);
try {
    // Use result
} finally {
    result.close();
}
```

## ProGuard Rules

If using ProGuard/R8, add these rules:

```proguard
-keep class com.openpajak.pph.** { *; }
-keepclassmembers class com.openpajak.pph.** {
    native <methods>;
}
```

## Troubleshooting

### Library Not Found

**Error**: `java.lang.UnsatisfiedLinkError: dlopen failed: library "libpph.so" not found`

**Solution**: Ensure `.so` files are in `src/main/jniLibs/<ABI>/`

### Wrong ABI

**Error**: Library loads but crashes immediately

**Solution**: Build for correct ABI. Use `adb shell getprop ro.product.cpu.abi` to check device ABI.

### JNI Method Not Found

**Error**: `java.lang.UnsatisfiedLinkError: No implementation found for native method`

**Solution**:
1. Ensure Java package matches JNI function names (`com_openpajak_pph_`)
2. Rebuild the library
3. Clean and rebuild Android project

### Initialization Failed

**Error**: Crash on first calculation

**Solution**: Call `PPHCalculator.initialize()` before any calculations

## Build Size

Library sizes per ABI (Release build, stripped):
- arm64-v8a: ~80 KB
- armeabi-v7a: ~70 KB
- x86: ~85 KB
- x86_64: ~90 KB

Total (all ABIs): ~325 KB

## Performance

Benchmark on mid-range Android device (Snapdragon 730):
- PPh21 calculation: 0.3-0.8 ms
- Money parsing: 0.02 ms
- Money formatting: 0.05 ms

Performance is comparable to native code, significantly faster than pure Java implementations.

## License

Same as main PPHC library. See LICENSE file in project root.

## Support

For issues: https://github.com/openpajak/pphc/issues
