#!/bin/bash
# Build script for Android using CMake and Android NDK

set -e

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building PPHC for Android${NC}"

# Check if ANDROID_NDK is set
if [ -z "$ANDROID_NDK" ]; then
    echo -e "${RED}Error: ANDROID_NDK environment variable not set${NC}"
    echo "Please set ANDROID_NDK to your Android NDK installation path:"
    echo "  export ANDROID_NDK=/path/to/android-ndk"
    echo ""
    echo "Or download Android NDK from:"
    echo "  https://developer.android.com/ndk/downloads"
    exit 1
fi

if [ ! -d "$ANDROID_NDK" ]; then
    echo -e "${RED}Error: ANDROID_NDK path does not exist: $ANDROID_NDK${NC}"
    exit 1
fi

echo "Using Android NDK: $ANDROID_NDK"

# Default build configuration
BUILD_TYPE="${1:-Release}"
API_LEVEL="${ANDROID_API_LEVEL:-21}"
BUILD_DIR_BASE="build-android"

# Clean if requested
if [ "$2" = "clean" ]; then
    echo -e "${YELLOW}Cleaning build directories...${NC}"
    rm -rf "$BUILD_DIR_BASE"
fi

# Android ABIs to build
ABIS=("arm64-v8a" "armeabi-v7a" "x86" "x86_64")

echo -e "${GREEN}Building for Android API $API_LEVEL${NC}"
echo -e "${GREEN}Build type: $BUILD_TYPE${NC}"
echo ""

# Build for each ABI
for ABI in "${ABIS[@]}"; do
    echo -e "${GREEN}Building for $ABI...${NC}"

    BUILD_DIR="$BUILD_DIR_BASE/$ABI"
    mkdir -p "$BUILD_DIR"

    cd "$BUILD_DIR"

    # Configure with CMake
    cmake ../.. \
        -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ABI" \
        -DANDROID_PLATFORM="android-$API_LEVEL" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_STATIC_LIBS=ON \
        -DBUILD_TESTS=OFF \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_CLI=OFF

    # Build
    cmake --build . --config "$BUILD_TYPE" -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

    cd - > /dev/null

    echo -e "${GREEN}✓ Built for $ABI${NC}"
    echo ""
done

echo -e "${GREEN}Build complete!${NC}"
echo ""
echo "Output files per ABI:"
for ABI in "${ABIS[@]}"; do
    echo "  $BUILD_DIR_BASE/$ABI/libpph/:"
    if [ -f "$BUILD_DIR_BASE/$ABI/libpph/libpph.so" ]; then
        SIZE=$(ls -lh "$BUILD_DIR_BASE/$ABI/libpph/libpph.so" | awk '{print $5}')
        echo -e "    ${GREEN}✓${NC} libpph.so ($SIZE)"
    fi
    if [ -f "$BUILD_DIR_BASE/$ABI/libpph/libpph.a" ]; then
        SIZE=$(ls -lh "$BUILD_DIR_BASE/$ABI/libpph/libpph.a" | awk '{print $5}')
        echo "    ✓ libpph.a ($SIZE)"
    fi
done

echo ""
echo "To use in Android Studio/Gradle project:"
echo "  1. Copy libpph.so files to: app/src/main/jniLibs/<ABI>/"
echo "  2. Copy Java files from android/src/main/java/ to your project"
echo "  3. Call PPHCalculator.initialize() before using the library"
echo ""
echo "Example structure:"
echo "  app/src/main/jniLibs/"
echo "    ├── arm64-v8a/libpph.so"
echo "    ├── armeabi-v7a/libpph.so"
echo "    ├── x86/libpph.so"
echo "    └── x86_64/libpph.so"
