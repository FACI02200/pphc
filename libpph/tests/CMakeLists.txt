# Test executables
add_executable(test_money test_money.c)
target_link_libraries(test_money pph_static)
add_test(NAME test_money COMMAND test_money)

add_executable(test_pph21 test_pph21.c)
target_link_libraries(test_pph21 pph_static)
add_test(NAME test_pph21 COMMAND test_pph21)
