#!/bin/bash
# Build script for WebAssembly/Emscripten

set -e

# Check if emscripten is available
if ! command -v emcc &> /dev/null; then
    echo "Error: emscripten not found. Please install emsdk first:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    exit 1
fi

echo "Building PPHC for WebAssembly..."
echo "Emscripten version: $(emcc --version | head -1)"

# Build type (default: Release)
BUILD_TYPE="${1:-Release}"

# Build directory
BUILD_DIR="build-wasm"

# Clean build directory if requested
if [ "$2" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with emcmake
echo "Configuring CMake for Emscripten..."
emcmake cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_STATIC_LIBS=ON \
    -DBUILD_TESTS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_CLI=OFF

# Build
echo "Building..."
emmake make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "Build complete!"
echo "Output files:"
echo "  JavaScript: $BUILD_DIR/libpph/pph.js"
echo "  WebAssembly: $BUILD_DIR/libpph/pph.wasm"
echo ""
echo "You can test the module in Node.js:"
echo "  node -e \"require('./$BUILD_DIR/libpph/pph.js')().then(m => console.log('Version:', m.UTF8ToString(m._pph_get_version())))\""
