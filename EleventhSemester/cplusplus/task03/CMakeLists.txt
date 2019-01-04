cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -Wformat -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -Wctor-dtor-privacy -Werror -pthread")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(part1 part1.cpp)

add_library(part2 part2.cpp)

add_executable(part2-main part2-main.cpp)
target_link_libraries(part2-main pthread part2)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(part2-test part2-test.cpp)
target_link_libraries(part2-test ${GTEST_LIBRARIES} pthread part2)
