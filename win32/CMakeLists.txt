# CMakeLists.txt for Win32 PPh 21 Calculator
# Windows NT 4.0+ compatible GUI application
# Links to PPHC static library

cmake_minimum_required(VERSION 3.24)

# Enable WATCOM_RUNTIME_LIBRARY property support
if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(pphcalc_win32 C)

# Set C standard
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED OFF)

# Windows NT 4.0+ target
add_definitions(-D_WIN32_WINNT=0x0400)
add_definitions(-DWINVER=0x0400)

# ANSI/MBCS build (not Unicode) for NT 4.0 compatibility
add_definitions(-D_MBCS)
remove_definitions(-DUNICODE)
remove_definitions(-D_UNICODE)

# Additional Windows definitions
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libpph/include
)

# Source files
set(PPHCALC_SOURCES
    src/main.c
    src/mainwnd.c
    src/controls.c
    src/calculation.c
    src/display.c
    src/formatting.c
    src/colors.c
    src/clipboard.c
    src/pph_w32_const.c
)

# Header files (for IDE)
set(PPHCALC_HEADERS
    include/pphcalc.h
    include/resource.h
    include/colors.h
)

# Create executable
add_executable(pphcalc WIN32 ${PPHCALC_SOURCES} ${PPHCALC_HEADERS})

# Link to PPHC static library
target_link_libraries(pphcalc
    pph_static          # PPHC static library
    comctl32            # Common controls
    comdlg32            # Common dialogs
    user32              # User interface
    gdi32               # Graphics
    kernel32            # Core Windows API
)

# Set output directory
set_target_properties(pphcalc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win32/bin
    OUTPUT_NAME "pphcalc"
)

# Compiler-specific flags
if(CMAKE_C_COMPILER_ID STREQUAL "Watcom")
    # OpenWatcom flags
    target_compile_options(pphcalc PRIVATE
        /wx                     # Warnings as errors
        /ox                     # Maximum optimization
        /bt=nt                  # Windows NT target
    )

    # Static runtime linking (no DLL dependency)
    set_property(TARGET pphcalc PROPERTY
        WATCOM_RUNTIME_LIBRARY "MultiThreaded"
    )

elseif(MINGW)
    # MinGW flags
    target_compile_options(pphcalc PRIVATE
        -Wall
        -Wextra
        -O2
        -municode=0             # ANSI mode
    )

    # MinGW linker flags
    set_target_properties(pphcalc PROPERTIES
        LINK_FLAGS "-mwindows -static-libgcc"
    )
endif()

# Installation
install(TARGETS pphcalc
    RUNTIME DESTINATION bin
)

# Copy PPHC library documentation to output directory (optional)
if(EXISTS ${CMAKE_SOURCE_DIR}/README.md)
    configure_file(${CMAKE_SOURCE_DIR}/README.md
        ${CMAKE_BINARY_DIR}/win32/bin/README_PPHC.txt COPYONLY)
endif()

# Build information
message(STATUS "Win32 PPh Calculator Configuration:")
message(STATUS "  Target OS: Windows NT 4.0+")
message(STATUS "  Character Set: ANSI/MBCS")
message(STATUS "  Library Linking: Static (pph_static)")
message(STATUS "  Subsystem: Windows GUI")
