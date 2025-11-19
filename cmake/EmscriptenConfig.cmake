# Emscripten/WebAssembly Configuration

if(NOT EMSCRIPTEN)
    return()
endif()

message(STATUS "Configuring for WebAssembly/Emscripten")

# Emscripten-specific settings
set(CMAKE_EXECUTABLE_SUFFIX ".js")

# Build options for WASM
option(WASM_ENABLE_SIMD "Enable WASM SIMD instructions" OFF)
option(WASM_ENABLE_THREADS "Enable WASM threads support" OFF)
option(WASM_STANDALONE "Build standalone WASM (not Node.js specific)" ON)

# Export all public API functions
set(EMSCRIPTEN_EXPORTED_FUNCTIONS
    "_pph_init"
    "_pph_get_version"
    "_pph_get_last_error"
    "_pph_money_add"
    "_pph_money_sub"
    "_pph_money_mul"
    "_pph_money_mul_int"
    "_pph_money_percent"
    "_pph_money_div"
    "_pph_money_min"
    "_pph_money_max"
    "_pph_money_cmp"
    "_pph_money_round_down_thousand"
    "_pph_money_floor"
    "_pph_money_to_string"
    "_pph_money_to_string_formatted"
    "_pph_percent_to_string"
    "_pph_money_from_string"
    "_pph_money_from_string_id"
    "_pph21_calculate"
    "_pph22_calculate"
    "_pph23_calculate"
    "_pph4_2_calculate"
    "_ppn_calculate"
    "_ppnbm_calculate"
    "_pph_result_free"
    "_pph_get_ptkp"
    "_pph_calculate_pasal17"
    "_pph_get_ter_bulanan_rate"
    "_pph_get_ter_harian_rate"
    "_malloc"
    "_free"
)

# Convert list to comma-separated string
string(REPLACE ";" "," EMSCRIPTEN_EXPORTED_FUNCTIONS_STR "${EMSCRIPTEN_EXPORTED_FUNCTIONS}")

# Base Emscripten link flags
set(EMSCRIPTEN_LINK_FLAGS
    "-s WASM=1"
    "-s EXPORTED_FUNCTIONS='[${EMSCRIPTEN_EXPORTED_FUNCTIONS_STR}]'"
    "-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\",\"getValue\",\"setValue\",\"UTF8ToString\",\"stringToUTF8\",\"lengthBytesUTF8\"]'"
    "-s ALLOW_MEMORY_GROWTH=1"
    "-s MODULARIZE=1"
    "-s EXPORT_NAME='createPPHModule'"
    "-s ENVIRONMENT='web,worker'"
    "--no-entry"
)

# Add optimization flags for release builds
if(CMAKE_BUILD_TYPE MATCHES "Release")
    list(APPEND EMSCRIPTEN_LINK_FLAGS
        "-O3"
        "-s ASSERTIONS=0"
        "--closure 1"
    )
else()
    list(APPEND EMSCRIPTEN_LINK_FLAGS
        "-O0"
        "-g"
        "-s ASSERTIONS=1"
        "-s SAFE_HEAP=1"
    )
endif()

# Add SIMD if enabled
if(WASM_ENABLE_SIMD)
    list(APPEND EMSCRIPTEN_LINK_FLAGS "-msimd128")
endif()

# Add threads if enabled
if(WASM_ENABLE_THREADS)
    list(APPEND EMSCRIPTEN_LINK_FLAGS
        "-s USE_PTHREADS=1"
        "-s PTHREAD_POOL_SIZE=4"
    )
endif()

# Convert list to space-separated string
string(REPLACE ";" " " EMSCRIPTEN_LINK_FLAGS_STR "${EMSCRIPTEN_LINK_FLAGS}")

# Apply to all targets
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS_STR}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS_STR}")

# Disable building executables and CLI for WASM
set(BUILD_CLI OFF CACHE BOOL "CLI not supported for WASM" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Examples not directly runnable in WASM" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Use static linking for WASM" FORCE)

message(STATUS "WASM configuration:")
message(STATUS "  SIMD: ${WASM_ENABLE_SIMD}")
message(STATUS "  Threads: ${WASM_ENABLE_THREADS}")
message(STATUS "  Standalone: ${WASM_STANDALONE}")
