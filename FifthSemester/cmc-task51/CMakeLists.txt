cmake_minimum_required (VERSION 2.6)
project (ElPro C ASM)

set (ELPRO_VERSION_MAJOR 0)
set (ELPRO_VERSION_MINOR 5)
set (ELPRO_OPTIMIZATION_LEVEL 2)

set (CMAKE_C_FLAGS "-O${ELPRO_OPTIMIZATION_LEVEL} -Wall -Werror -Wno-pointer-sign -std=gnu11 -lm")
set (CMAKE_CXX_FLAGS "-O${ELPRO_OPTIMIZATION_LEVEL} -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -std=gnu++0x -lm")


include_directories ("${PROJECT_BINARY_DIR}")

add_executable (elpro main.c)

add_library (profile profile.c)
add_library (sum sum.c)
set (ELPRO_LIBS ${ELPRO_LIBS} profile sum)

if (UNIX AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86_64)
    set (HAVE_RDTSC 1)
    add_library (op_rdtsc op_rdtsc64.s)
    set (ELPRO_LIBS ${ELPRO_LIBS} op_rdtsc)
elseif (UNIX AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86)
    set (HAVE_RDTSC 1)
    add_library (op_rdtsc op_rdtsc32.s)
    set (ELPRO_LIBS ${ELPRO_LIBS} op_rdtsc)
else ()
    set (HAVE_RDTSC 0)
endif ()

configure_file (
  "${PROJECT_SOURCE_DIR}/ElproConfig.h.in"
  "${PROJECT_BINARY_DIR}/ElproConfig.h"
)

target_link_libraries (elpro ${ELPRO_LIBS})
