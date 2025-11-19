# CLI executable
add_executable(pphc
    src/main.c
)

# Link against libpph
if(BUILD_SHARED_LIBS)
    target_link_libraries(pphc PRIVATE pph_shared)
else()
    target_link_libraries(pphc PRIVATE pph_static)
endif()

# Install executable
install(TARGETS pphc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
