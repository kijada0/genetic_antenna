cmake_minimum_required(VERSION 3.25)
project(genetic_antenna C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(genetic_antenna main.c)

target_link_libraries(genetic_antenna m)

# ADD NECPP
set(NECPP_DIR "d:/Documents/Library/necpp/src")
include_directories(${NECPP_DIR})
link_directories(${NECPP_DIR})
target_link_libraries(genetic_antenna necpp)

