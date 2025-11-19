# Building PPHC

This document provides detailed platform-specific build instructions for the PPHC library.

## Prerequisites

| Platform | Requirements |
|----------|--------------|
| **Unix/Linux** | GCC/Clang, CMake 3.10+, make |
| **macOS** | Xcode Command Line Tools, CMake 3.10+ |
| **iOS** | Xcode, CMake 3.10+ |
| **Windows MSVC** | Visual Studio 2019+, CMake 3.10+ |
| **Windows OpenWatcom** | OpenWatcom 1.9+, CMake 3.10+ |
| **Windows MinGW** | MinGW-w64, CMake 3.10+ |

---

## Unix/Linux (GCC/Clang)

### Install Dependencies

```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake

# Fedora/RHEL
sudo dnf install gcc cmake make

# Arch Linux
sudo pacman -S base-devel cmake
```

### Build

```bash
cd pphc
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Install

```bash
sudo make install
```

Default installation:
- Libraries: `/usr/local/lib/libpph.so`, `/usr/local/lib/libpph.a`
- Headers: `/usr/local/include/pph/*.h`
- Executable: `/usr/local/bin/pphc`

### Custom Install Prefix

```bash
cmake -DCMAKE_INSTALL_PREFIX=/opt/pph -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

---

## macOS (Clang)

### Install Dependencies

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake via Homebrew
brew install cmake
```

### Build Framework

```bash
cd pphc
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)
```

**Output:**
- `libpph/pph.framework` - macOS framework (drag into Xcode)
- `cli/pphc` - command-line tool

### Install

```bash
sudo make install
```

Default installation:
- Framework: `/Library/Frameworks/pph.framework`
- Executable: `/usr/local/bin/pphc`

---

## iOS

### iOS Device (arm64)

```bash
cd pphc
mkdir build-ios && cd build-ios
cmake -G Xcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -DBUILD_CLI=OFF \
    ..
cmake --build . --config Release
```

**Output:** `libpph/Release-iphoneos/pph.framework`

### iOS Simulator (x86_64 + arm64)

```bash
cd pphc
mkdir build-ios-sim && cd build-ios-sim
cmake -G Xcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_OSX_SYSROOT=iphonesimulator \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -DBUILD_CLI=OFF \
    ..
cmake --build . --config Release
```

**Output:** `libpph/Release-iphonesimulator/pph.framework`

### Create XCFramework (Universal)

```bash
xcodebuild -create-xcframework \
    -framework build-ios/libpph/Release-iphoneos/pph.framework \
    -framework build-ios-sim/libpph/Release-iphonesimulator/pph.framework \
    -output pph.xcframework
```

**Output:** `pph.xcframework` (drag into Xcode, works on all devices and simulators)

---

## Windows (MSVC)

### Visual Studio 2019+

#### Using Visual Studio IDE

1. Open Visual Studio
2. File → Open → CMake → Select `pphc/CMakeLists.txt`
3. Build → Build All
4. Output in `out/build/x64-Release/`

#### Using Command Line

```batch
REM Open "x64 Native Tools Command Prompt for VS 2019"

cd pphc
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

**Output:**
- `libpph\Release\pph.dll` (with version 0.1a in properties)
- `libpph\Release\pph.lib` (import library)
- `libpph\Release\pph_static.lib`
- `cli\Release\pphc.exe`

#### Using NMake

```batch
REM Open "x64 Native Tools Command Prompt for VS 2019"

cd pphc
mkdir build
cd build
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
nmake
```

### Install

```batch
REM Run as Administrator
cmake --build . --target install
```

Default installation: `C:\Program Files\pph\`

---

## Windows (OpenWatcom 1.9)

### Setup Environment

```batch
set WATCOM=C:\WATCOM
set PATH=%WATCOM%\binnt;%WATCOM%\binw;%PATH%
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\nt
```

### Build (32-bit)

```batch
cd pphc
mkdir build
cd build
cmake -G "Watcom WMake" -DCMAKE_BUILD_TYPE=Release ..
wmake
```

**Output:**
- `libpph\pph.dll` (with version 0.1a)
- `libpph\pph.lib` (import library)
- `libpph\pph_s.lib` (static, small model)
- `cli\pphc.exe`

### Build (16-bit DOS)

For DOS builds, OpenWatcom requires manual wmake files. The 32-bit version is recommended.

---

## Windows (MinGW)

### Install MinGW-w64

Download from: https://www.mingw-w64.org/downloads/

Add to PATH: `C:\mingw64\bin`

### Build

```batch
cd pphc
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
```

**Output:**
- `libpph\libpph.dll` (MinGW keeps 'lib' prefix)
- `libpph\libpph.dll.a` (import library)
- `libpph\libpph.a` (static)
- `cli\pphc.exe`

---

## Build Options

All platforms support these CMake options:

```bash
cmake \
    -DBUILD_SHARED_LIBS=ON   \  # Build shared library (default ON)
    -DBUILD_STATIC_LIBS=ON   \  # Build static library (default ON)
    -DBUILD_CLI=ON            \  # Build pphc command-line tool (default ON)
    -DBUILD_TESTS=ON          \  # Build test suite (default ON)
    -DBUILD_EXAMPLES=ON       \  # Build examples (default ON)
    ..
```

Example - static library only:

```bash
cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON ..
```

---

## Running Tests

### All Platforms

```bash
cd build
ctest
```

Or:

```bash
cmake --build . --target test
```

### Run Specific Test

```bash
# Unix/Linux/macOS
./libpph/tests/test_money
./libpph/tests/test_pph21

# Windows
libpph\tests\Release\test_money.exe
libpph\tests\Release\test_pph21.exe
```

---

## Troubleshooting

### OpenWatcom: "Unable to open stdint.h"

Ensure WATCOM environment variables are set correctly:

```batch
set WATCOM=C:\WATCOM
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt
```

### iOS: Code Signing Error

For development builds, use:

```bash
cmake -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED=NO ..
```

### Windows: pph.dll not found

Copy `pph.dll` to the same directory as `pphc.exe`, or add to PATH.

### macOS: Framework validation errors

Ensure `Info.plist.in` exists in `libpph/resources/`.

---

## Cross-Compilation

### Linux → Windows (MinGW)

```bash
sudo apt-get install mingw-w64
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-toolchain.cmake ..
```

Create `cmake/mingw-toolchain.cmake`:

```cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
```

---

## Verifying Build

### Check DLL Version (Windows)

Right-click `pph.dll` → Properties → Details

Should show version **0.1a**.

### Check Framework Version (macOS/iOS)

```bash
plutil -p pph.framework/Resources/Info.plist
```

Should show `CFBundleShortVersionString = "0.1a"`.

### Run Example

```bash
# Unix/Linux/macOS
./examples/example_pph21

# Windows
examples\Release\example_pph21.exe
```

---

## Performance Notes

- **DOS (8086 @ 4.77 MHz):** ~200ms per PPh21 calculation
- **DOS (386 @ 33 MHz):** ~5ms per calculation
- **Modern systems:** <1ms per calculation

---

## Support

For build issues: https://github.com/openpajak/pphc/issues
