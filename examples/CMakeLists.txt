# Example programs
add_executable(example_250m_with_bonuses
    example_250m_with_bonuses.c
)

set_target_properties(
    example_250m_with_bonuses PROPERTIES
    WATCOM_RUNTIME_LIBRARY "MultiThreaded"
)

add_executable(custom_allocator
    custom_allocator.c
)

set_target_properties(
    custom_allocator PROPERTIES
    WATCOM_RUNTIME_LIBRARY "MultiThreaded"
)


# Link against libpph
if(BUILD_SHARED_LIBS)
    target_link_libraries(example_250m_with_bonuses PRIVATE pph_shared)
    target_link_libraries(custom_allocator PRIVATE pph_shared)
else()
    target_link_libraries(example_250m_with_bonuses PRIVATE pph_static)
    target_link_libraries(custom_allocator PRIVATE pph_static)
endif()
