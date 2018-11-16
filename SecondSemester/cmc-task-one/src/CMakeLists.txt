cmake_minimum_required (VERSION 2.8)
enable_testing ()
set (CMAKE_C_FLAGS "-O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu99")

project (cmc-task-one)

add_library (apmath STATIC apmath.c)

add_executable (cmc-task-one main.c)
target_link_libraries (cmc-task-one apmath)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (tests)

