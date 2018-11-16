cmake_minimum_required (VERSION 2.8.11)
project (gear CXX C)

set (CMAKE_CXX_FLAGS "-O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo -std=gnu++1y")
set (CMAKE_VERBOSE_MAKEFILE on)

set (${PROJECT_NAME}_SOURCES "")

set (${PROJECT_NAME}_LIBS model controller view)
foreach (LIBNAME ${${PROJECT_NAME}_LIBS})
    file (GLOB LIB_SOURCES "${LIBNAME}/*.cpp")
    set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${LIB_SOURCES})
endforeach (LIBNAME)

add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries (${PROJECT_NAME} ${CMAKE_DL_LIBS})

set (FLTK_SKIP_OPENGL on)
set (FLTK_SKIP_FORMS on)
set (FLTK_SKIP_IMAGES on)
set (FLTK_SKIP_FLUID on)
find_package (FLTK REQUIRED)
target_link_libraries(${PROJECT_NAME} ${FLTK_LIBRARIES})

set (EXTERNAL_LIBS EasyBMP whereami)
foreach (LIBNAME ${EXTERNAL_LIBS})
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/externals/${LIBNAME}/include")
    add_library (${LIBNAME} "${CMAKE_CURRENT_SOURCE_DIR}/externals/${LIBNAME}/${LIBNAME}")
    target_link_libraries (${PROJECT_NAME} ${LIBNAME})
endforeach (LIBNAME)

add_subdirectory (plugins)
