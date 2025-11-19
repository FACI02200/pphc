# Example programs
add_executable(example_250m_with_bonuses
    example_250m_with_bonuses.c
)

# Link against libpph
if(BUILD_SHARED_LIBS)
    target_link_libraries(example_250m_with_bonuses PRIVATE pph_shared)
else()
    target_link_libraries(example_250m_with_bonuses PRIVATE pph_static)
endif()
