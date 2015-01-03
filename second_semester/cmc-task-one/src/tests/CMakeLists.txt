add_executable (test_stdfun test_stdfun.c)
target_link_libraries (test_stdfun apmath m)

add_executable (test_matan test_matan.c)
target_link_libraries (test_matan apmath m)

add_executable (test_numan test_numan.c)
target_link_libraries (test_numan apmath m)

add_test (test_stdfun test_stdfun)
add_test (test_matan test_matan)
add_test (test_numan test_numan)

