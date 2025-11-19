function(configure_platform)
    if(WIN32)
        message(STATUS "Configuring for Windows")

        # Windows-specific definitions
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

        # Set proper library extensions
        if(MSVC OR WATCOM)
            set(CMAKE_SHARED_LIBRARY_PREFIX "" PARENT_SCOPE)
        endif()

    elseif(UNIX AND NOT APPLE)
        message(STATUS "Configuring for Linux")

        # Linux-specific settings
        set(CMAKE_POSITION_INDEPENDENT_CODE ON PARENT_SCOPE)

    elseif(APPLE)
        message(STATUS "Configuring for macOS/iOS")

        # macOS/iOS-specific settings
        set(CMAKE_MACOSX_RPATH ON PARENT_SCOPE)

        if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
            message(STATUS "Building for iOS")
            set(IOS TRUE PARENT_SCOPE)
        endif()
    endif()
endfunction()
