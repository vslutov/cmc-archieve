add_executable (test_is_equal test_is_equal.c)
target_link_libraries (test_is_equal polynomial)

add_test (test_is_equal test_is_equal)

