cmake_minimum_required (VERSION 3.0)

project (blackhole CXX C)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# set (CMAKE_VERBOSE_MAKEFILE on)
set (CMAKE_CXX_FLAGS "-O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel -Woverloaded-virtual -Wsign-promo -std=gnu++1y")

set (${PROJECT_NAME}_SOURCES "")

file (GLOB LIB_SOURCES "src/*.cpp")
set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${LIB_SOURCES})

add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

find_library(FREEIMAGE_LIBRARIES freeimage)
find_file(FREEIMAGE_HEADER FreeImage.h)
get_filename_component(FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_HEADER} DIRECTORY)

target_compile_definitions(${PROJECT_NAME} PUBLIC
                           ${OPENGL_DEFINITIONS}
                           ${GLUT_DEFINITIONS}
                           ${GLEW_DEFINITIONS})

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${OPENGL_INCLUDE_DIRS}
                           ${GLUT_INCLUDE_DIRS}
                           ${GLEW_INCLUDE_DIRS}
                           ${FREEIMAGE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${GLUT_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${FREEIMAGE_LIBRARIES})


# add a target to generate API documentation with Doxygen
set(DOXYGEN_SKIP_DOT true)
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen")
endif(BUILD_DOCUMENTATION)

file(GLOB_RECURSE in_file_list  ${CMAKE_CURRENT_SOURCE_DIR}/data/*)
foreach(in_file ${in_file_list})
    string(REGEX REPLACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} out_file ${in_file})
    add_custom_command (
        TARGET     ${PROJECT_NAME}
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${in_file} ${out_file}
    )
endforeach(in_file)
