cmake_minimum_required (VERSION 2.8)
enable_testing ()
set (CMAKE_C_FLAGS "-O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu99")

project (cmc-task-two)

add_library (polynomial STATIC polynomial.c)

add_executable (cmc-task-two main.c)
target_link_libraries (cmc-task-two polynomial)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (tests)

