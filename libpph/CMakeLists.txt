# Library source files
set(LIBPPH_SOURCES
    src/pph_money.c
    src/pph_constants.c
    src/pph_breakdown.c
    src/pph21.c
    src/pph22.c
    src/pph23.c
    src/pph4_2.c
    src/ppn.c
    src/ppnbm.c
)

# Add JNI wrapper for Android
if(ANDROID)
    list(APPEND LIBPPH_SOURCES
        ../android/jni/pph_jni.c
    )
endif()

set(LIBPPH_HEADERS
    include/pph/pph_calculator.h
    include/pph/pph_types.h
    include/pph/pph_export.h
)

# Windows resource file
if(WIN32)
    set(LIBPPH_RESOURCES
        resources/version.rc
    )
endif()

# Shared library
if(BUILD_SHARED_LIBS)
    add_library(pph_shared SHARED ${LIBPPH_SOURCES})

    # Add resources on Windows
    if(WIN32)
        target_sources(pph_shared PRIVATE ${LIBPPH_RESOURCES})
    endif()

    # Platform-specific output naming
    if(WIN32)
        if(MSVC OR WATCOM)
            set_target_properties(pph_shared PROPERTIES
                OUTPUT_NAME "pph"
                PREFIX ""
            )
            set_target_properties(pph_shared PROPERTIES 
                WATCOM_RUNTIME_LIBRARY "MultiThreaded"
            )
        elseif(MINGW)
            set_target_properties(pph_shared PROPERTIES
                OUTPUT_NAME "pph"
            )
        endif()
    else()
        set_target_properties(pph_shared PROPERTIES
            OUTPUT_NAME "pph"
        )
    endif()

    # Apple Framework configuration
    if(APPLE)
        set_target_properties(pph_shared PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION A
            MACOSX_FRAMEWORK_IDENTIFIER com.openpajak.pph
            VERSION ${PROJECT_VERSION}
            SOVERSION 1
            PUBLIC_HEADER "${LIBPPH_HEADERS}"
        )

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in")
            set_target_properties(pph_shared PROPERTIES
                MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in"
            )
        endif()
    endif()

    # Include directories
    target_include_directories(pph_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    # Export symbols for DLL
    target_compile_definitions(pph_shared
        PRIVATE PPH_BUILD_DLL
    )

    # Version information
    set_target_properties(pph_shared PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )

    # Install targets
    install(TARGETS pph_shared
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION /Library/Frameworks
    )
endif()

# WebAssembly library (Emscripten)
if(EMSCRIPTEN)
    add_executable(pph_wasm ${LIBPPH_SOURCES})

    set_target_properties(pph_wasm PROPERTIES
        OUTPUT_NAME "pph"
        SUFFIX ".js"
    )

    # Include directories
    target_include_directories(pph_wasm
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    # Install WASM output
    install(TARGETS pph_wasm
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # Also install the .wasm file
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/pph.wasm
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        OPTIONAL
    )
endif()

# Static library
if(BUILD_STATIC_LIBS)
    add_library(pph_static STATIC ${LIBPPH_SOURCES})

    # Platform-specific output naming
    if(WIN32)
        if(MSVC)
            set_target_properties(pph_static PROPERTIES
                OUTPUT_NAME "pph_static"
            )
        elseif(WATCOM)
            set_target_properties(pph_static PROPERTIES
                OUTPUT_NAME "pph_s"
            )

            set_target_properties(pph_static PROPERTIES 
                WATCOM_RUNTIME_LIBRARY "MultiThreaded"
            )

        elseif(MINGW)
            set_target_properties(pph_static PROPERTIES
                OUTPUT_NAME "pph"
            )
        endif()
    else()
        set_target_properties(pph_static PROPERTIES
            OUTPUT_NAME "pph"
        )
    endif()

    # Include directories
    target_include_directories(pph_static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    # Install targets
    install(TARGETS pph_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

# Install headers (temporarily disabled for testing)
# install(DIRECTORY include/pph
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# Add tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
