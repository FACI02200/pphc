# Apple Framework Configuration

function(configure_apple_framework target)
    if(NOT APPLE)
        return()
    endif()

    set_target_properties(${target} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A
        MACOSX_FRAMEWORK_IDENTIFIER com.openpajak.pph
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )

    # Set Info.plist if available
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in")
        set_target_properties(${target} PROPERTIES
            MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in"
        )
    endif()

    message(STATUS "Configured ${target} as Apple Framework")
endfunction()
