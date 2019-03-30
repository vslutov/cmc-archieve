cmake_minimum_required (VERSION 3.5)
project (OpenCLTask1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCL REQUIRED)
find_library(IL IL)
find_library(ILU ILU)
find_library(ILUT ILUT)

# Set opencl include dir here
set(OpenCL_INCLUDE_DIRS "/usr/local/cuda-8.0/targets/x86_64-linux/include")

include_directories(${OpenCL_INCLUDE_DIRS})
message(WARNING "OpenCL include dir manually set to: ${OpenCL_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME} main.cxx)
target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES} ${IL} ${ILU} ${ILUT})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ApplyFilter.cl ${CMAKE_CURRENT_BINARY_DIR}/ApplyFilter.cl COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bliss.png ${CMAKE_CURRENT_BINARY_DIR}/bliss.png COPYONLY)

find_package(OpenCL REQUIRED)
