# Android NDK Configuration

if(NOT ANDROID)
    return()
endif()

message(STATUS "Configuring for Android")
message(STATUS "  Android ABI: ${ANDROID_ABI}")
message(STATUS "  Android Platform: ${ANDROID_PLATFORM}")
message(STATUS "  Android NDK: ${ANDROID_NDK}")

# Android-specific settings
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set C standard for Android
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Android platform detection
if(ANDROID_PLATFORM_LEVEL LESS 21)
    message(WARNING "Android API level ${ANDROID_PLATFORM_LEVEL} is very old. Consider targeting API 21+")
endif()

# Disable building executables for Android
set(BUILD_CLI OFF CACHE BOOL "CLI not supported for Android" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Examples not directly runnable on Android" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Tests should run on host, not Android" FORCE)

# Force shared library for JNI
set(BUILD_SHARED_LIBS ON CACHE BOOL "Android requires shared libraries for JNI" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Also build static library" FORCE)

# Android-specific compiler flags
add_compile_options(-ffunction-sections -fdata-sections)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(-O3 -DNDEBUG)
    # Link-time optimization
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    add_compile_options(-O0 -g)
endif()

# Linker flags for size optimization
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,--as-needed")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -Wl,--gc-sections")

message(STATUS "Android configuration complete")
